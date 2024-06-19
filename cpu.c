#include "./cpu.h"
#include <stdint.h>
#include <stdio.h>

instruction_lookup[16*16] = {
  {"BRK", 7},
}  

void create_cpu(CPU6502 *cpu, Bus *bus) {
  cpu->A = 0;
  cpu->X = 0;
  cpu->Y = 0;
  cpu->PC = 0;
  cpu->SP = 0xfd;
      
  cpu->cycles = 7;
  cpu->bus = bus;
} 

int get_flag(CPU6502 *cpu, CPUStatusFlags flag) {
  return (cpu->SR >> flag) & 0x1;
}

void set_flag(CPU6502 *cpu, CPUStatusFlags flag, int bool) {
  if(bool) {
    cpu->SR |= 0x1 << flag;
  } else {
    cpu->SR &= ~(0x1 << flag);
  }
}

void instruction_lookup() {
  
}

void ADC(CPU6502 *cpu) {
  
}

void AND(CPU6502 *cpu) {}
void ASL(CPU6502 *cpu) {}
void BCC(CPU6502 *cpu) {}
void BCS(CPU6502 *cpu) {}
void BEQ(CPU6502 *cpu) {}
void BIT(CPU6502 *cpu) {}
void BMI(CPU6502 *cpu) {}
void BNE(CPU6502 *cpu) {}
void BPL(CPU6502 *cpu) {}
void BRK(CPU6502 *cpu) {}
void BVC(CPU6502 *cpu) {}
void BVS(CPU6502 *cpu) {}
void CLC(CPU6502 *cpu) {}
void CLD(CPU6502 *cpu) {}
void CLI(CPU6502 *cpu) {}
void CLV(CPU6502 *cpu) {}
void CMP(CPU6502 *cpu) {}
void CPX(CPU6502 *cpu) {}
void CPY(CPU6502 *cpu) {}
void DEC(CPU6502 *cpu) {}
void DEX(CPU6502 *cpu) {}
void DEY(CPU6502 *cpu) {}
void EOR(CPU6502 *cpu) {}
void INC(CPU6502 *cpu) {}
void INX(CPU6502 *cpu) {}
void INY(CPU6502 *cpu) {}
void JMP(CPU6502 *cpu) {}
void JSR(CPU6502 *cpu) {}

void LDA(CPU6502 *cpu) {
  cpu->A = 0xf;
  set_flag(cpu, Z, cpu->A == 0x00);
};

void LDX(CPU6502 *cpu) {}
void LDY(CPU6502 *cpu) {}
void LSR(CPU6502 *cpu) {}
void NOP(CPU6502 *cpu) {}
void ORA(CPU6502 *cpu) {}
void PHA(CPU6502 *cpu) {}
void PHP(CPU6502 *cpu) {}
void PLA(CPU6502 *cpu) {}
void PLP(CPU6502 *cpu) {}
void ROL(CPU6502 *cpu) {}
void ROR(CPU6502 *cpu) {}
void RTI(CPU6502 *cpu) {}
void RTS(CPU6502 *cpu) {}
void SBC(CPU6502 *cpu) {}
void SEC(CPU6502 *cpu) {}
void SED(CPU6502 *cpu) {}
void SEI(CPU6502 *cpu) {}
void STA(CPU6502 *cpu) {}
void STX(CPU6502 *cpu) {}
void STY(CPU6502 *cpu) {}
void TAX(CPU6502 *cpu) {}
void TAY(CPU6502 *cpu) {}
void TSX(CPU6502 *cpu) {}
void TXA(CPU6502 *cpu) {}
void TXS(CPU6502 *cpu) {}
void TYA(CPU6502 *cpu) {}
void XXX(CPU6502 *cpu) {}

void clock(CPU6502 *cpu) {
  if(cpu->cycles == 0) {
    opcode = cpu->PC;
  }
};

void reset(CPU6502 *cpu) {
  cpu->PC = 0xffffffff;
  cpu->A = 0;
  cpu->X = 0;
  cpu->Y = 0;
  cpu->SP = 0xFD;

};

void irq() {};
void nmi() {};

void execute_opcode(CPU6502 *cpu, uint8_t opcode) {
      switch(opcode) {
        case 0x00: // BRK
        case 0x01: // ORA (Indirect,X)
        case 0x05: // ORA Zero Page
        case 0x06: // ASL Zero Page
        case 0x08: // PHP
        case 0x09: // ORA Immediate
        case 0x0A: // ASL Accumulator
        case 0x0D: // ORA Absolute
        case 0x0E: // ASL Absolute
        case 0x10: // BPL
        case 0x11: // ORA (Indirect),Y
        case 0x15: // ORA Zero Page,X
        case 0x16: // ASL Zero Page,X
        case 0x18: // CLC
        case 0x19: // ORA Absolute,Y
        case 0x1D: // ORA Absolute,X
        case 0x1E: // ASL Absolute,X
        case 0x20: // JSR
        case 0x21: // AND (Indirect,X)
        case 0x24: // BIT Zero Page
        case 0x25: // AND Zero Page
        case 0x26: // ROL Zero Page
        case 0x28: // PLP
        case 0x29: // AND Immediate
        case 0x2A: // ROL Accumulator
        case 0x2C: // BIT Absolute
        case 0x2D: // AND Absolute
        case 0x2E: // ROL Absolute
        case 0x30: // BMI
        case 0x31: // AND (Indirect),Y
        case 0x35: // AND Zero Page,X
        case 0x36: // ROL Zero Page,X
        case 0x38: // SEC
        case 0x39: // AND Absolute,Y
        case 0x3D: // AND Absolute,X
        case 0x3E: // ROL Absolute,X
        case 0x40: // RTI
        case 0x41: // EOR (Indirect,X)
        case 0x45: // EOR Zero Page
        case 0x46: // LSR Zero Page
        case 0x48: // PHA
        case 0x49: // EOR Immediate
        case 0x4A: // LSR Accumulator
        case 0x4C: // JMP Absolute
        case 0x4D: // EOR Absolute
        case 0x4E: // LSR Absolute
        case 0x50: // BVC
        case 0x51: // EOR (Indirect),Y
        case 0x55: // EOR Zero Page,X
        case 0x56: // LSR Zero Page,X
        case 0x58: // CLI
        case 0x59: // EOR Absolute,Y
        case 0x5D: // EOR Absolute,X
        case 0x5E: // LSR Absolute,X
        case 0x60: // RTS
        case 0x61: // ADC (Indirect,X)
        case 0x65: // ADC Zero Page
        case 0x66: // ROR Zero Page
        case 0x68: // PLA
        case 0x69: // ADC Immediate
        case 0x6A: // ROR Accumulator
        case 0x6C: // JMP (Indirect)
        case 0x6D: // ADC Absolute
        case 0x6E: // ROR Absolute
        case 0x70: // BVS
        case 0x71: // ADC (Indirect),Y
        case 0x75: // ADC Zero Page,X
        case 0x76: // ROR Zero Page,X
        case 0x78: // SEI
        case 0x79: // ADC Absolute,Y
        case 0x7D: // ADC Absolute,X
        case 0x7E: // ROR Absolute,X
        case 0x81: // STA (Indirect,X)
        case 0x84: // STY Zero Page
        case 0x85: // STA Zero Page
        case 0x86: // STX Zero Page
        case 0x88: // DEY
        case 0x8A: // TXA
        case 0x8C: // STY Absolute
        case 0x8D: // STA Absolute
        case 0x8E: // STX Absolute
        case 0x90: // BCC
        case 0x91: // STA (Indirect),Y
        case 0x94: // STY Zero Page,X
        case 0x95: // STA Zero Page,X
        case 0x96: // STX Zero Page,Y
        case 0x98: // TYA
        case 0x99: // STA Absolute,Y
        case 0x9A: // TXS
        case 0x9D: // STA Absolute,X
        case 0xA0: // LDY Immediate
        case 0xA1: // LDA (Indirect,X)
        case 0xA2: // LDX Immediate
        case 0xA4: // LDY Zero Page
        case 0xA5: // LDA Zero Page
        case 0xA6: // LDX Zero Page
        case 0xA8: // TAY
        case 0xA9: // LDA Immediate
        case 0xAA: // TAX
        case 0xAC: // LDY Absolute
        case 0xAD: // LDA Absolute
        case 0xAE: // LDX Absolute
        case 0xB0: // BCS
        case 0xB1: // LDA (Indirect),Y
        case 0xB4: // LDY Zero Page,X
        case 0xB5: // LDA Zero Page,X
        case 0xB6: // LDX Zero Page,Y
        case 0xB8: // CLV
        case 0xB9: // LDA Absolute,Y
        case 0xBA: // TSX
        case 0xBC: // LDY Absolute,X
        case 0xBD: // LDA Absolute,X
        case 0xBE: // LDX Absolute,Y
        case 0xC0: // CPY Immediate
        case 0xC1: // CMP (Indirect,X)
        case 0xC4: // CPY Zero Page
        case 0xC5: // CMP Zero Page
        case 0xC6: // DEC Zero Page
        case 0xC8: // INY
        case 0xC9: // CMP Immediate
        case 0xCA: // DEX
        case 0xCC: // CPY Absolute
        case 0xCD: // CMP Absolute
        case 0xCE: // DEC Absolute
        case 0xD0: // BNE
        case 0xD1: // CMP (Indirect),Y
        case 0xD5: // CMP Zero Page,X
        case 0xD6: // DEC Zero Page,X
        case 0xD8: // CLD
        case 0xD9: // CMP Absolute,Y
        case 0xDD: // CMP Absolute,X
        case 0xDE: // DEC Absolute,X
        case 0xE0: // CPX Immediate
        case 0xE1: // SBC (Indirect,X)
        case 0xE4: // CPX Zero Page
        case 0xE5: // SBC Zero Page
        case 0xE6: // INC Zero Page
        case 0xE8: // INX
        case 0xE9: // SBC Immediate
        case 0xEA: // NOP
        case 0xEC: // CPX Absolute
        case 0xED: // SBC Absolute
        case 0xEE: // INC Absolute
        case 0xF0: // BEQ
        case 0xF1: // SBC (Indirect),Y
        case 0xF5: // SBC Zero Page,X
        case 0xF6: // INC Zero Page,X
        case 0xF8: // SED
        case 0xF9: // SBC Absolute,Y
        case 0xFD: // SBC Absolute,X
        case 0xFE: // INC Absolute,X
            break;
        default:
            printf("Unknown opcode: 0x%02X\n", opcode);
            break;
    }
   
}
