#include "cpu.h"

#include "apu.h"
#include "cartridge.h"
#include "controller.h"
#include "ppu.h"

#include <iostream>
#include <stdexcept>

namespace nes
{
	namespace memory
	{
		template<auto Operation>
		int get_cpu_map(const uint16_t addr)
		{
			auto in_range = [addr](const auto lower, const auto upper) {
				return (addr >= lower) && (addr <= upper);
			};

			if constexpr (Operation == Read)
			{
				if (addr <= 0x1FFF)
					return CPU_RAM;
				else if (in_range(0x2000, 0x3FFF))
					return PPU_Access;
				else if (in_range(0x4000, 0x4013) || addr == 0x4015)
					return APU_Access;
				else if (addr == 0x4016)
					return Controller_1;
				else if (addr == 0x4017)
					return Controller_2;
				else if (in_range(0x4018, 0x401F))
					return Unknown;
				else if (in_range(0x4020, 0x5FFF))
					return Unknown;
				else if (in_range(0x6000, 0xFFFF))
					return Cartridge;
			}
			else if constexpr (Operation == Write)
			{
				if (addr <= 0x1FFF)
					return CPU_RAM;
				else if (in_range(0x2000, 0x3FFF))
					return PPU_Access;
				else if (in_range(0x4000, 0x4013) || addr == 0x4015)
					return APU_Access;
				else if (addr == 0x4014)
					return OAMDMA;
				else if (addr == 0x4016)
					return Controller;
				else if (addr == 0x4017)
					return APU_Access;
				else if (in_range(0x4018, 0x401F))
					return Unknown;
				else if (in_range(0x4020, 0xFFFF))
					return Cartridge;
			}
			return Unknown;
		}
		template int get_cpu_map<Read>(uint16_t);
		template int get_cpu_map<Write>(uint16_t);
	}

	bool state::check_flags(uint8_t flags) const
	{
		return (ps & flags) == flags;
	}
	void state::set_flags(uint8_t flags)
	{
		ps |= flags;
	}
	void state::clear_flags(uint8_t flags)
	{
		ps &= ~flags;
	}
	void state::update_nz(uint8_t value)
	{
		clear_flags(flags::Zero | flags::Negative);

		if (value == 0)
			set_flags(flags::Zero);
		else if (value & 0x80)
			set_flags(flags::Negative);
	}
	void state::set_a(uint8_t value)
	{
		update_nz(a = value);
	}
	void state::set_x(uint8_t value)
	{
		update_nz(x = value);
	}
	void state::set_y(uint8_t value)
	{
		update_nz(y = value);
	}
	void state::set_pc(uint16_t addr)
	{
		pc = addr;
	}
	void state::set_ps(uint8_t value)
	{
		ps = value & 0xCF;
	}

	cpu::cpu(nes::ppu & ppu, nes::apu & apu, nes::controller & controller, nes::cartridge & cartridge) :
	  ppu{ ppu },
	  apu{ apu },
	  controller{ controller },
	  cartridge{ cartridge }
	{}

	void cpu::reset()
	{
		remaining_cycles = 0;
		ram.fill(0);
		state.set_ps(0x34);
		INT_RST();
	}
	void cpu::set_nmi(bool value)
	{
		state.nmi_flag = value;
	}
	void cpu::set_irq(bool value)
	{
		state.irq_flag = value;
	}
	void cpu::dma_oam(uint8_t addr)
	{
		for (size_t i = 0; i < 256; ++i) memory_write(0x2014, memory_read((addr * 0x100) + i));
	}
	void cpu::run_frame()
	{
		remaining_cycles += total_cycles;

		while (remaining_cycles > 0)
		{
			if (state.nmi_flag)
				INT_NMI();
			else if (state.irq_flag && !state.check_flags(flags::Interrupt))
				INT_IRQ();
			execute();
		}

		// state.cycle_count = 0;
	}
	void cpu::tick()
	{
		ppu.step();
		ppu.step();
		ppu.step();
		--remaining_cycles;

		// ++state.cycle_count;
	}
	uint8_t cpu::read(const uint16_t addr) const
	{
		if (addr & 0x8000) return cartridge.prg_read(addr);
		if (~addr & 0x4000) return (addr & 0x2000) ? ppu.read(addr % 0x8) : ram[addr % 0x800];
		if (addr & 0x2000) return cartridge.prg_read(addr);
		if (addr & 0x1FE0) throw std::runtime_error(std::string{ "Illegal read at " } + std::to_string(addr));
		if (~addr & 0x0010) return apu.read(elapsed());
		if (addr & 0x0008) throw std::runtime_error(std::string{ "Illegal read at " } + std::to_string(addr));
		if (~addr & 0x0004) return apu.read(elapsed());
		if (addr & 0x0002) return controller.read(addr & 0x0001);
		if (addr & 0x0001) return apu.read(elapsed());
		throw std::runtime_error(std::string{ "Illegal read at " } + std::to_string(addr));
	}
	void cpu::write(uint16_t addr, uint8_t value)
	{
		if (addr & 0x8000) return cartridge.prg_write(addr, value);
		if (~addr & 0x4000)
		{
			if (addr & 0x2000)
				ppu.write(addr, value);
			else
				ram[addr % 0x800] = value;
			return;
		}
		if (addr & 0x3FE0) return cartridge.prg_write(addr, value);
		if (~addr & 0x0010) return apu.write(elapsed(), addr, value);
		if (addr & 0x0008) throw std::runtime_error(std::string{ "Illegal write at " } + std::to_string(addr));
		if (~addr & 0x0004) return apu.write(elapsed(), addr, value);
		if (addr & 0x0001) return apu.write(elapsed(), addr, value);
		if (addr & 0x0002) return controller.write(value & 0x0001);
		dma_oam(value);
	}
	uint8_t cpu::memory_read(uint16_t addr)
	{
		tick();
		return read(addr);
	}
	void cpu::memory_write(uint16_t addr, uint8_t value)
	{
		tick();
		write(addr, value);
	}
	uint8_t cpu::peek(uint16_t addr) const
	{
		return read(addr);
	}
	uint16_t cpu::peek_imm() const
	{
		return state.pc + 1;
	}
	uint16_t cpu::peek_rel() const
	{
		return state.pc + 1;
	}
	uint16_t cpu::peek_zp() const
	{
		return peek(peek_imm());
	}
	uint16_t cpu::peek_zpx() const
	{
		return (peek_zp() + state.x) & 0xFF;
	}
	uint16_t cpu::peek_zpy() const
	{
		return (peek_zp() + state.y) & 0xFF;
	}
	uint16_t cpu::peek_ab() const
	{
		auto base_addr = peek_imm();
		return (peek(base_addr + 1) << 8) | peek(base_addr);
	}
	uint16_t cpu::peek_abx() const
	{
		auto base_addr = peek_ab();
		return base_addr + state.x;
	}
	uint16_t cpu::peek_aby() const
	{
		auto base_addr = peek_ab();
		return base_addr + state.y;
	}
	uint16_t cpu::peek_ind() const
	{
		auto base_addr = peek_ab();
		return peek(base_addr) | (peek((base_addr & 0xFF00) | ((base_addr + 1) % 0x100)) << 8);
	}
	uint16_t cpu::peek_indx() const
	{
		auto base_addr = peek_zpx();
		return (peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr);
	}
	uint16_t cpu::peek_indy() const
	{
		auto base_addr = peek_zp();
		return ((peek((base_addr + 1) & 0xFF) << 8) | peek(base_addr)) + state.y;
	}
	int cpu::elapsed() const
	{
		return total_cycles - remaining_cycles;
	}
}
