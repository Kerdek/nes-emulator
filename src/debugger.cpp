#include "debugger.h"

#include "cpu.h"
#include "memory_mapper.h"

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <array>
#include <iomanip>
#include <sstream>
#include <string_view>

namespace nes
{
    uint16_t debugger::peek_imm() const
    {
        return cpu.pc + 1;
    }
    uint16_t debugger::peek_zp() const
    {
        return memory_mapper.read(peek_imm());
    }
    uint16_t debugger::peek_zpx() const
    {
        return (peek_zp() + cpu.x) & 0xFF;
    }
    uint16_t debugger::peek_zpy() const
    {
        return (peek_zp() + cpu.y) & 0xFF;
    }
    uint16_t debugger::peek_ab() const
    {
        auto base_addr = peek_imm();
        return (memory_mapper.read(base_addr + 1) << 8) | memory_mapper.read(base_addr);
    }
    uint16_t debugger::peek_abx() const
    {
        auto base_addr = peek_ab();
        return base_addr + cpu.x;
    }
    uint16_t debugger::peek_aby() const
    {
        auto base_addr = peek_ab();
        return base_addr + cpu.y;
    }
    uint16_t debugger::peek_ind() const
    {
        auto base_addr = peek_ab();
        return memory_mapper.read(base_addr) | (memory_mapper.read((base_addr & 0xFF00) | ((base_addr + 1) % 0x100)) << 8);
    }
    uint16_t debugger::peek_indx() const
    {
        auto base_addr = peek_zpx();
        return (memory_mapper.read((base_addr + 1) & 0xFF) << 8) | memory_mapper.read(base_addr);
    }
    uint16_t debugger::peek_indy() const
    {
        auto base_addr = peek_zp();
        return ((memory_mapper.read((base_addr + 1) & 0xFF) << 8) | memory_mapper.read(base_addr)) + cpu.y;
    }

    void debugger::nestest()
    {
        constexpr std::array<std::string_view, 0x100> instruction = {
            // 0     1      2      3       4       5      6      7       8      9      A       B       C       D      E      F
            "BRK",
            "ORA",
            "inv",
            "*SLO",
            "*NOP",
            "ORA",
            "ASL",
            "*SLO",
            "PHP",
            "ORA",
            "ASL",
            "inv",
            "*NOP",
            "ORA",
            "ASL",
            "*SLO",    // 0
            "BPL",
            "ORA",
            "inv",
            "*SLO",
            "*NOP",
            "ORA",
            "ASL",
            "*SLO",
            "CLC",
            "ORA",
            "*NOP",
            "*SLO",
            "*NOP",
            "ORA",
            "ASL",
            "*SLO",    // 1
            "JSR",
            "AND",
            "inv",
            "*RLA",
            "BIT",
            "AND",
            "ROL",
            "*RLA",
            "PLP",
            "AND",
            "ROL",
            "inv",
            "BIT",
            "AND",
            "ROL",
            "*RLA",    // 2
            "BMI",
            "AND",
            "inv",
            "*RLA",
            "*NOP",
            "AND",
            "ROL",
            "*RLA",
            "SEC",
            "AND",
            "*NOP",
            "*RLA",
            "*NOP",
            "AND",
            "ROL",
            "*RLA",    // 3
            "RTI",
            "EOR",
            "inv",
            "*SRE",
            "*NOP",
            "EOR",
            "LSR",
            "*SRE",
            "PHA",
            "EOR",
            "LSR",
            "inv",
            "JMP",
            "EOR",
            "LSR",
            "*SRE",    // 4
            "BVC",
            "EOR",
            "inv",
            "*SRE",
            "*NOP",
            "EOR",
            "LSR",
            "*SRE",
            "CLI",
            "EOR",
            "*NOP",
            "*SRE",
            "*NOP",
            "EOR",
            "LSR",
            "*SRE",    // 5
            "RTS",
            "ADC",
            "inv",
            "*RRA",
            "*NOP",
            "ADC",
            "ROR",
            "*RRA",
            "PLA",
            "ADC",
            "ROR",
            "inv",
            "JMP",
            "ADC",
            "ROR",
            "*RRA",    // 6
            "BVS",
            "ADC",
            "inv",
            "*RRA",
            "*NOP",
            "ADC",
            "ROR",
            "*RRA",
            "SEI",
            "ADC",
            "*NOP",
            "*RRA",
            "*NOP",
            "ADC",
            "ROR",
            "*RRA",    // 7
            "*NOP",
            "STA",
            "inv",
            "*SAX",
            "STY",
            "STA",
            "STX",
            "*SAX",
            "DEY",
            "inv",
            "TXA",
            "inv",
            "STY",
            "STA",
            "STX",
            "*SAX",    // 8
            "BCC",
            "STA",
            "inv",
            "inv",
            "STY",
            "STA",
            "STX",
            "*SAX",
            "TYA",
            "STA",
            "TXS",
            "inv",
            "inv",
            "STA",
            "inv",
            "inv",    // 9
            "LDY",
            "LDA",
            "LDX",
            "*LAX",
            "LDY",
            "LDA",
            "LDX",
            "*LAX",
            "TAY",
            "LDA",
            "TAX",
            "inv",
            "LDY",
            "LDA",
            "LDX",
            "*LAX",    // A
            "BCS",
            "LDA",
            "inv",
            "*LAX",
            "LDY",
            "LDA",
            "LDX",
            "*LAX",
            "CLV",
            "LDA",
            "TSX",
            "inv",
            "LDY",
            "LDA",
            "LDX",
            "*LAX",    // B
            "CPY",
            "CMP",
            "inv",
            "*DCP",
            "CPY",
            "CMP",
            "DEC",
            "*DCP",
            "INY",
            "CMP",
            "DEX",
            "inv",
            "CPY",
            "CMP",
            "DEC",
            "*DCP",    // C
            "BNE",
            "CMP",
            "inv",
            "*DCP",
            "*NOP",
            "CMP",
            "DEC",
            "*DCP",
            "CLD",
            "CMP",
            "*NOP",
            "*DCP",
            "*NOP",
            "CMP",
            "DEC",
            "*DCP",    // D
            "CPX",
            "SBC",
            "inv",
            "*ISB",
            "CPX",
            "SBC",
            "INC",
            "*ISB",
            "INX",
            "SBC",
            "NOP",
            "*SBC",
            "CPX",
            "SBC",
            "INC",
            "*ISB",    // E
            "BEQ",
            "SBC",
            "inv",
            "*ISB",
            "*NOP",
            "SBC",
            "INC",
            "*ISB",
            "SED",
            "SBC",
            "*NOP",
            "*ISB",
            "*NOP",
            "SBC",
            "INC",
            "*ISB",    // F
        };

        constexpr std::array<addr_mode2, 0x100> addressing_mode = {
            // 0  1     2    3     4    5    6    7    8     9    A     B    C    D    E    F
            impl,
            indx,
            inv,
            indx,
            zp,
            zp,
            zp,
            zp,
            impl,
            imm,
            acc,
            inv,
            ab,
            ab,
            ab,
            ab,    // 0
            rel,
            indy,
            inv,
            indy,
            zpx,
            zpx,
            zpx,
            zpx,
            impl,
            aby,
            impl,
            aby,
            abx,
            abx,
            abx,
            abx,    // 1
            ab,
            indx,
            inv,
            indx,
            zp,
            zp,
            zp,
            zp,
            impl,
            imm,
            acc,
            inv,
            ab,
            ab,
            ab,
            ab,    // 2
            rel,
            indy,
            inv,
            indy,
            zpx,
            zpx,
            zpx,
            zpx,
            impl,
            aby,
            impl,
            aby,
            abx,
            abx,
            abx,
            abx,    // 3
            impl,
            indx,
            inv,
            indx,
            zp,
            zp,
            zp,
            zp,
            impl,
            imm,
            acc,
            inv,
            ab,
            ab,
            ab,
            ab,    // 4
            rel,
            indy,
            inv,
            indy,
            zpx,
            zpx,
            zpx,
            zpx,
            impl,
            aby,
            impl,
            aby,
            abx,
            abx,
            abx,
            abx,    // 5
            impl,
            indx,
            inv,
            indx,
            zp,
            zp,
            zp,
            zp,
            impl,
            imm,
            acc,
            inv,
            ind,
            ab,
            ab,
            ab,    // 6
            rel,
            indy,
            inv,
            indy,
            zpx,
            zpx,
            zpx,
            zpx,
            impl,
            aby,
            impl,
            aby,
            abx,
            abx,
            abx,
            abx,    // 7
            imm,
            indx,
            inv,
            indx,
            zp,
            zp,
            zp,
            zp,
            impl,
            inv,
            impl,
            inv,
            ab,
            ab,
            ab,
            ab,    // 8
            rel,
            indy,
            inv,
            inv,
            zpx,
            zpx,
            zpy,
            zpy,
            impl,
            aby,
            impl,
            inv,
            inv,
            abx,
            inv,
            inv,    // 9
            imm,
            indx,
            imm,
            indx,
            zp,
            zp,
            zp,
            zp,
            impl,
            imm,
            impl,
            inv,
            ab,
            ab,
            ab,
            ab,    // A
            rel,
            indy,
            inv,
            indy,
            zpx,
            zpx,
            zpy,
            zpy,
            impl,
            aby,
            impl,
            inv,
            abx,
            abx,
            aby,
            aby,    // B
            imm,
            indx,
            inv,
            indx,
            zp,
            zp,
            zp,
            zp,
            impl,
            imm,
            impl,
            inv,
            ab,
            ab,
            ab,
            ab,    // C
            rel,
            indy,
            inv,
            indy,
            zpx,
            zpx,
            zpx,
            zpx,
            impl,
            aby,
            impl,
            aby,
            abx,
            abx,
            abx,
            abx,    // D
            imm,
            indx,
            inv,
            indx,
            zp,
            zp,
            zp,
            zp,
            impl,
            imm,
            impl,
            imm,
            ab,
            ab,
            ab,
            ab,    // E
            rel,
            indy,
            inv,
            indy,
            zpx,
            zpx,
            zpx,
            zpx,
            impl,
            aby,
            impl,
            aby,
            abx,
            abx,
            abx,
            abx,    // F
        };

        auto read_word    = [this](uint16_t addr) -> uint16_t { return memory_mapper.read(addr + 1) << 8 | memory_mapper.read(addr); };
        auto read_word_zp = [this](uint16_t addr) -> uint16_t { return memory_mapper.read((addr + 1) & 0xFF) << 8 | memory_mapper.read(addr); };
        auto inst         = instruction[memory_mapper.read(cpu.pc)];
        auto addr_m       = addressing_mode[memory_mapper.read(cpu.pc)];

        std::stringstream ss;

        ss << fmt::format("{:04X}  {:02X} ", cpu.pc, memory_mapper.read(cpu.pc));

        uint8_t  arg8   = memory_mapper.read(cpu.pc + 1);
        uint8_t  arg8_2 = memory_mapper.read(cpu.pc + 2);
        uint16_t arg16  = arg8 | (arg8_2 << 8);

        switch (addr_m)
        {
        case addr_mode2::ab:
        case addr_mode2::abx:
        case addr_mode2::aby:
        case addr_mode2::ind:
            ss << fmt::format("{:02X} {:02X} ", arg8, arg8_2);
            break;
        case addr_mode2::indy:
        case addr_mode2::indx:
        case addr_mode2::zp:
        case addr_mode2::zpx:
        case addr_mode2::zpy:
        case addr_mode2::rel:
        case addr_mode2::imm:
            ss << fmt::format("{:02X}    ", arg8);
            break;
        default:
            ss << "      ";
            break;
        }
        ss << fmt::format("{:>4s} ", inst);
        ss << std::left << std::setw(28) << std::setfill(' ');
        switch (addr_m)
        {
        case addr_mode2::impl:
        {
            ss << " ";
            break;
        }
        case addr_mode2::acc:
            if (inst == "LSR" || inst == "ASL" || inst == "ROR" || inst == "ROL")
                ss << "A";
            else
                ss << " ";
            break;
        case addr_mode2::imm:
        {
            ss << fmt::format("#${:02X}", memory_mapper.read(peek_imm()));
            break;
        }
        case addr_mode2::zp:
        {
            auto addr = memory_mapper.read(peek_imm());
            ss << fmt::format("${:02X} = {:02X}", addr, memory_mapper.read(addr));
            break;
        }
        case addr_mode2::zpx:
        {
            auto zp_addr = peek_zp();
            auto addr    = peek_zpx();    // (zp_addr + state.x) & 0xFF
            ss << fmt::format("${:02X},X @ {:02X} = {:02X}", zp_addr, addr, memory_mapper.read(addr));
            break;
        }
        case addr_mode2::zpy:
        {
            auto zp_addr = peek_zp();
            auto addr    = peek_zpy();
            ss << fmt::format("${:02X},Y @ {:02X} = {:02X}", zp_addr, addr, memory_mapper.read(addr));
            break;
        }
        case addr_mode2::rel:
        {
            ss << fmt::format("${:04X}", cpu.pc + 2 + static_cast<int8_t>(memory_mapper.read(peek_imm())));
            break;
        }
        case addr_mode2::ab:
        {
            auto addr = read_word(peek_imm());
            if (inst == "JMP" || inst == "JSR")
                ss << fmt::format("${:04X}", addr);
            else
                ss << fmt::format("${:04X} = {:02X}", addr, memory_mapper.read(addr));
            break;
        }
        case addr_mode2::abx:
        {
            ss << fmt::format("${:04X},X @ {:04X} = {:02X}", arg16, uint16_t(arg16 + cpu.x), memory_mapper.read(arg16 + cpu.x));
            break;
        }
        case addr_mode2::aby:
        {
            ss << fmt::format("${:04X},Y @ {:04X} = {:02X}", arg16, uint16_t(arg16 + cpu.y), memory_mapper.read(arg16 + cpu.y));
            break;
        }
        case addr_mode2::ind:
        {
            ss << fmt::format("(${:04X}) = {:04X}", peek_ab(), peek_ind());
            break;
        }
        case addr_mode2::indx:
        {
            ss << fmt::format("(${:02X},X) @ {:02X} = {:04X} = {:02X}", arg8, uint8_t(cpu.x + arg8), read_word_zp((cpu.x + arg8) % 0x100), memory_mapper.read(read_word_zp((cpu.x + arg8) % 0x100)));
            break;
        }
        case addr_mode2::indy:
        {
            ss << fmt::format("(${:02X}),Y = {:04X} @ {:04X} = {:02X}", arg8, read_word_zp(arg8), uint16_t(read_word_zp(arg8) + cpu.y), memory_mapper.read(read_word_zp(arg8) + cpu.y));
            break;
        }
        default:
            ss << " ";
            break;
        }

        ss << fmt::format("A:{:02X} X:{:02X} Y:{:02X} P:{:02X} SP:{:02X} CYC:{:3d}\n", cpu.a, cpu.x, cpu.y, (cpu.p | 0x20), cpu.s, (cpu.cycle - 7) * 3 % 341);

        nestest_log << ss.str();

        if (cpu.pc == 0xC66E)
        {
            nestest_log.close();
            exit(0);
        }
    }
}
