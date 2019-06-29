#pragma once

#include <array>
#include <cstdint>

namespace nes
{
	class ppu;
	class memory_mapper;

	class cpu
	{
		friend class debugger;

		nes::ppu &			 ppu;
		nes::memory_mapper & memory_mapper;

		uint8_t  a  = 0;
		uint8_t  x  = 0;
		uint8_t  y  = 0;
		uint16_t pc = 0;
		uint8_t  s  = 0;
		uint8_t  p  = 0;

		bool nmi_flag = false;
		bool irq_flag = false;

		bool get_flags(uint8_t) const;
		void set_flags(uint8_t);
		void clear_flags(uint8_t);
		void update_nz(uint8_t);

		void set_a(uint8_t);
		void set_x(uint8_t);
		void set_y(uint8_t);
		void set_pc(uint16_t);
		void set_ps(uint8_t);
		void set_irq();

		void clock();

		uint8_t read(uint16_t);
		void	write(uint16_t, uint8_t);
		void	oam_dma(uint8_t value);

		int cycle		= 0;
		int final_cycle = 29780;

		cpu(cpu const &) = delete;
		cpu(cpu &&)		 = delete;
		cpu & operator=(cpu const &) = delete;
		cpu & operator=(cpu &&) = delete;

	public:
		cpu(nes::ppu & ppu, nes::memory_mapper & memory_mapper);

		void reset();
		void run_frame();

		void set_nmi();

	private:
		//
		// All functions defined here are
		// implemented in cpu_instructions.cpp
		//

		void step();

		/* Instructions */

		//
		// Auxiliary
		//

		template<auto Mode>
		uint16_t get_operand();

		void	add(uint8_t);
		uint8_t shift_left(uint8_t);	 // Arithmetic left shift
		uint8_t shift_right(uint8_t);	// Logical right shift
		uint8_t rotate_left(uint8_t);
		uint8_t rotate_right(uint8_t);

		void compare(uint8_t, uint8_t);

		void branch(bool);

		void	push(uint8_t);
		uint8_t pop();

		bool crosses_page(uint16_t, uint8_t) const;
		bool crosses_page(uint16_t, int8_t) const;

		//
		// Storage
		//

		template<auto Mode>
		void LDA();
		template<auto Mode>
		void LDX();
		template<auto Mode>
		void LDY();

		template<auto Mode>
		void STA();
		template<auto Mode>
		void STX();
		template<auto Mode>
		void STY();

		void TAX();
		void TAY();
		void TSX();
		void TXA();
		void TXS();
		void TYA();

		//
		// Math
		//

		template<auto Mode>
		void ADC();
		template<auto Mode>
		void SBC();
		template<auto Mode>
		void INC();
		template<auto Mode>
		void DEC();
		void INX();
		void INY();
		void DEX();
		void DEY();

		//
		// Bitwise
		//

		template<auto Mode>
		void AND();
		template<auto Mode>
		void ORA();
		template<auto Mode>
		void EOR();
		template<auto Mode>
		void LSR();
		template<auto Mode>
		void ASL();
		template<auto Mode>
		void ROL();
		template<auto Mode>
		void ROR();

		//
		// Flags
		//

		void CLC();
		void CLD();
		void CLI();
		void CLV();
		void SEC();
		void SED();
		void SEI();

		template<auto Mode>
		void CMP();
		template<auto Mode>
		void CPX();
		template<auto Mode>
		void CPY();
		template<auto Mode>
		void BIT();

		//
		// Jumps and branches
		//

		template<auto Mode>
		void JMP();
		void JSR();
		void RTS();
		void RTI();

		void BCC();
		void BCS();
		void BEQ();
		void BMI();
		void BNE();
		void BPL();
		void BVC();
		void BVS();

		//
		// Stack
		//

		void PHA();
		void PLA();
		void PHP();
		void PLP();

		//
		// System
		//

		void INT_NMI();
		void INT_RST();
		void INT_IRQ();
		void INT_BRK();

		void NOP();

		//
		// Unofficial instructions
		//

		template<auto Mode>
		void NOP();
		template<auto Mode>
		void LAX();	// LDA then TXA
		template<auto Mode>
		void SAX();	// A & X
		template<auto Mode>
		void DCP();	// DEC then CMP
		template<auto Mode>
		void ISB();	// INC then SBC
		template<auto Mode>
		void SLO();	// ASL then ORA
		template<auto Mode>
		void RLA();	// ROL then AND
		template<auto Mode>
		void SRE();	// LSR then EOR
		template<auto Mode>
		void RRA();	// ROR then ADC
	};
}
