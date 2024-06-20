#include "./cpu.h"
#include <stdint.h>
#include <stdio.h>

static Instructions LOOKUP[16 * 16] = {
    {"BRK", BRK, IMP, 7}, {"ORA", ORA, IZX, 6}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 8}, {"???", NOP, IMP, 3}, {"ORA", ORA, ZP0, 3},
    {"ASL", ASL, ZP0, 5}, {"???", XXX, IMP, 5}, {"PHP", PHP, IMP, 3},
    {"ORA", ORA, IMM, 2}, {"ASL", ASL, IMP, 2}, {"???", XXX, IMP, 2},
    {"???", NOP, IMP, 4}, {"ORA", ORA, ABS, 4}, {"ASL", ASL, ABS, 6},
    {"???", XXX, IMP, 6}, {"BPL", BPL, REL, 2}, {"ORA", ORA, IZY, 5},
    {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4},
    {"ORA", ORA, ZPX, 4}, {"ASL", ASL, ZPX, 6}, {"???", XXX, IMP, 6},
    {"CLC", CLC, IMP, 2}, {"ORA", ORA, ABY, 4}, {"???", NOP, IMP, 2},
    {"???", XXX, IMP, 7}, {"???", NOP, IMP, 4}, {"ORA", ORA, ABX, 4},
    {"ASL", ASL, ABX, 7}, {"???", XXX, IMP, 7}, {"JSR", JSR, ABS, 6},
    {"AND", AND, IZX, 6}, {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8},
    {"BIT", BIT, ZP0, 3}, {"AND", AND, ZP0, 3}, {"ROL", ROL, ZP0, 5},
    {"???", XXX, IMP, 5}, {"PLP", PLP, IMP, 4}, {"AND", AND, IMM, 2},
    {"ROL", ROL, IMP, 2}, {"???", XXX, IMP, 2}, {"BIT", BIT, ABS, 4},
    {"AND", AND, ABS, 4}, {"ROL", ROL, ABS, 6}, {"???", XXX, IMP, 6},
    {"BMI", BMI, REL, 2}, {"AND", AND, IZY, 5}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4}, {"AND", AND, ZPX, 4},
    {"ROL", ROL, ZPX, 6}, {"???", XXX, IMP, 6}, {"SEC", SEC, IMP, 2},
    {"AND", AND, ABY, 4}, {"???", NOP, IMP, 2}, {"???", XXX, IMP, 7},
    {"???", NOP, IMP, 4}, {"AND", AND, ABX, 4}, {"ROL", ROL, ABX, 7},
    {"???", XXX, IMP, 7}, {"RTI", RTI, IMP, 6}, {"EOR", EOR, IZX, 6},
    {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8}, {"???", NOP, IMP, 3},
    {"EOR", EOR, ZP0, 3}, {"LSR", LSR, ZP0, 5}, {"???", XXX, IMP, 5},
    {"PHA", PHA, IMP, 3}, {"EOR", EOR, IMM, 2}, {"LSR", LSR, IMP, 2},
    {"???", XXX, IMP, 2}, {"JMP", JMP, ABS, 3}, {"EOR", EOR, ABS, 4},
    {"LSR", LSR, ABS, 6}, {"???", XXX, IMP, 6}, {"BVC", BVC, REL, 2},
    {"EOR", EOR, IZY, 5}, {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8},
    {"???", NOP, IMP, 4}, {"EOR", EOR, ZPX, 4}, {"LSR", LSR, ZPX, 6},
    {"???", XXX, IMP, 6}, {"CLI", CLI, IMP, 2}, {"EOR", EOR, ABY, 4},
    {"???", NOP, IMP, 2}, {"???", XXX, IMP, 7}, {"???", NOP, IMP, 4},
    {"EOR", EOR, ABX, 4}, {"LSR", LSR, ABX, 7}, {"???", XXX, IMP, 7},
    {"RTS", RTS, IMP, 6}, {"ADC", ADC, IZX, 6}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 8}, {"???", NOP, IMP, 3}, {"ADC", ADC, ZP0, 3},
    {"ROR", ROR, ZP0, 5}, {"???", XXX, IMP, 5}, {"PLA", PLA, IMP, 4},
    {"ADC", ADC, IMM, 2}, {"ROR", ROR, IMP, 2}, {"???", XXX, IMP, 2},
    {"JMP", JMP, IND, 5}, {"ADC", ADC, ABS, 4}, {"ROR", ROR, ABS, 6},
    {"???", XXX, IMP, 6}, {"BVS", BVS, REL, 2}, {"ADC", ADC, IZY, 5},
    {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4},
    {"ADC", ADC, ZPX, 4}, {"ROR", ROR, ZPX, 6}, {"???", XXX, IMP, 6},
    {"SEI", SEI, IMP, 2}, {"ADC", ADC, ABY, 4}, {"???", NOP, IMP, 2},
    {"???", XXX, IMP, 7}, {"???", NOP, IMP, 4}, {"ADC", ADC, ABX, 4},
    {"ROR", ROR, ABX, 7}, {"???", XXX, IMP, 7}, {"???", NOP, IMP, 2},
    {"STA", STA, IZX, 6}, {"???", NOP, IMP, 2}, {"???", XXX, IMP, 6},
    {"STY", STY, ZP0, 3}, {"STA", STA, ZP0, 3}, {"STX", STX, ZP0, 3},
    {"???", XXX, IMP, 3}, {"DEY", DEY, IMP, 2}, {"???", NOP, IMP, 2},
    {"TXA", TXA, IMP, 2}, {"???", XXX, IMP, 2}, {"STY", STY, ABS, 4},
    {"STA", STA, ABS, 4}, {"STX", STX, ABS, 4}, {"???", XXX, IMP, 4},
    {"BCC", BCC, REL, 2}, {"STA", STA, IZY, 6}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 6}, {"STY", STY, ZPX, 4}, {"STA", STA, ZPX, 4},
    {"STX", STX, ZPY, 4}, {"???", XXX, IMP, 4}, {"TYA", TYA, IMP, 2},
    {"STA", STA, ABY, 5}, {"TXS", TXS, IMP, 2}, {"???", XXX, IMP, 5},
    {"???", NOP, IMP, 5}, {"STA", STA, ABX, 5}, {"???", XXX, IMP, 5},
    {"???", XXX, IMP, 5}, {"LDY", LDY, IMM, 2}, {"LDA", LDA, IZX, 6},
    {"LDX", LDX, IMM, 2}, {"???", XXX, IMP, 6}, {"LDY", LDY, ZP0, 3},
    {"LDA", LDA, ZP0, 3}, {"LDX", LDX, ZP0, 3}, {"???", XXX, IMP, 3},
    {"TAY", TAY, IMP, 2}, {"LDA", LDA, IMM, 2}, {"TAX", TAX, IMP, 2},
    {"???", XXX, IMP, 2}, {"LDY", LDY, ABS, 4}, {"LDA", LDA, ABS, 4},
    {"LDX", LDX, ABS, 4}, {"???", XXX, IMP, 4}, {"BCS", BCS, REL, 2},
    {"LDA", LDA, IZY, 5}, {"???", XXX, IMP, 2}, {"???", XXX, IMP, 5},
    {"LDY", LDY, ZPX, 4}, {"LDA", LDA, ZPX, 4}, {"LDX", LDX, ZPY, 4},
    {"???", XXX, IMP, 4}, {"CLV", CLV, IMP, 2}, {"LDA", LDA, ABY, 4},
    {"TSX", TSX, IMP, 2}, {"???", XXX, IMP, 4}, {"LDY", LDY, ABX, 4},
    {"LDA", LDA, ABX, 4}, {"LDX", LDX, ABY, 4}, {"???", XXX, IMP, 4},
    {"CPY", CPY, IMM, 2}, {"CMP", CMP, IZX, 6}, {"???", NOP, IMP, 2},
    {"???", XXX, IMP, 8}, {"CPY", CPY, ZP0, 3}, {"CMP", CMP, ZP0, 3},
    {"DEC", DEC, ZP0, 5}, {"???", XXX, IMP, 5}, {"INY", INY, IMP, 2},
    {"CMP", CMP, IMM, 2}, {"DEX", DEX, IMP, 2}, {"???", XXX, IMP, 2},
    {"CPY", CPY, ABS, 4}, {"CMP", CMP, ABS, 4}, {"DEC", DEC, ABS, 6},
    {"???", XXX, IMP, 6}, {"BNE", BNE, REL, 2}, {"CMP", CMP, IZY, 5},
    {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4},
    {"CMP", CMP, ZPX, 4}, {"DEC", DEC, ZPX, 6}, {"???", XXX, IMP, 6},
    {"CLD", CLD, IMP, 2}, {"CMP", CMP, ABY, 4}, {"NOP", NOP, IMP, 2},
    {"???", XXX, IMP, 7}, {"???", NOP, IMP, 4}, {"CMP", CMP, ABX, 4},
    {"DEC", DEC, ABX, 7}, {"???", XXX, IMP, 7}, {"CPX", CPX, IMM, 2},
    {"SBC", SBC, IZX, 6}, {"???", NOP, IMP, 2}, {"???", XXX, IMP, 8},
    {"CPX", CPX, ZP0, 3}, {"SBC", SBC, ZP0, 3}, {"INC", INC, ZP0, 5},
    {"???", XXX, IMP, 5}, {"INX", INX, IMP, 2}, {"SBC", SBC, IMM, 2},
    {"NOP", NOP, IMP, 2}, {"???", SBC, IMP, 2}, {"CPX", CPX, ABS, 4},
    {"SBC", SBC, ABS, 4}, {"INC", INC, ABS, 6}, {"???", XXX, IMP, 6},
    {"BEQ", BEQ, REL, 2}, {"SBC", SBC, IZY, 5}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4}, {"SBC", SBC, ZPX, 4},
    {"INC", INC, ZPX, 6}, {"???", XXX, IMP, 6}, {"SED", SED, IMP, 2},
    {"SBC", SBC, ABY, 4}, {"NOP", NOP, IMP, 2}, {"???", XXX, IMP, 7},
    {"???", NOP, IMP, 4}, {"SBC", SBC, ABX, 4}, {"INC", INC, ABX, 7},
    {"???", XXX, IMP, 7},
};

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
  if (bool) {
    cpu->SR |= 0x1 << flag;
  } else {
    cpu->SR &= ~(0x1 << flag);
  }
}

void instruction_lookup() {}

/*
 * Addressing Mode
 */

uint8_t IMP() {

}

/*
* Instructions
*/

uint8_t ADC(CPU6502 *cpu) {}

uint8_t AND(CPU6502 *cpu) {}
uint8_t ASL(CPU6502 *cpu) {}
uint8_t BCC(CPU6502 *cpu) {}
uint8_t BCS(CPU6502 *cpu) {}
uint8_t BEQ(CPU6502 *cpu) {}
uint8_t BIT(CPU6502 *cpu) {}
uint8_t BMI(CPU6502 *cpu) {}
uint8_t BNE(CPU6502 *cpu) {}
uint8_t BPL(CPU6502 *cpu) {}
uint8_t BRK(CPU6502 *cpu) {}
uint8_t BVC(CPU6502 *cpu) {}
uint8_t BVS(CPU6502 *cpu) {}
uint8_t CLC(CPU6502 *cpu) {}
uint8_t CLD(CPU6502 *cpu) {}
uint8_t CLI(CPU6502 *cpu) {}
uint8_t CLV(CPU6502 *cpu) {}
uint8_t CMP(CPU6502 *cpu) {}
uint8_t CPX(CPU6502 *cpu) {}
uint8_t CPY(CPU6502 *cpu) {}
uint8_t DEC(CPU6502 *cpu) {}
uint8_t DEX(CPU6502 *cpu) {}
uint8_t DEY(CPU6502 *cpu) {}
uint8_t EOR(CPU6502 *cpu) {}
uint8_t INC(CPU6502 *cpu) {}
uint8_t INX(CPU6502 *cpu) {}
uint8_t INY(CPU6502 *cpu) {}
uint8_t JMP(CPU6502 *cpu) {}
uint8_t JSR(CPU6502 *cpu) {}

uint8_t LDA(CPU6502 *cpu) {
  cpu->A = 0xf;
  set_flag(cpu, Z, cpu->A == 0x00);
};

uint8_t LDX(CPU6502 *cpu) {}
uint8_t LDY(CPU6502 *cpu) {}
uint8_t LSR(CPU6502 *cpu) {}
uint8_t NOP(CPU6502 *cpu) {}
uint8_t ORA(CPU6502 *cpu) {}
uint8_t PHA(CPU6502 *cpu) {}
uint8_t PHP(CPU6502 *cpu) {}
uint8_t PLA(CPU6502 *cpu) {}
uint8_t PLP(CPU6502 *cpu) {}
uint8_t ROL(CPU6502 *cpu) {}
uint8_t ROR(CPU6502 *cpu) {}
uint8_t RTI(CPU6502 *cpu) {}
uint8_t RTS(CPU6502 *cpu) {}
uint8_t SBC(CPU6502 *cpu) {}
uint8_t SEC(CPU6502 *cpu) {}
uint8_t SED(CPU6502 *cpu) {}
uint8_t SEI(CPU6502 *cpu) {}
uint8_t STA(CPU6502 *cpu) {}
uint8_t STX(CPU6502 *cpu) {}
uint8_t STY(CPU6502 *cpu) {}
uint8_t TAX(CPU6502 *cpu) {}
uint8_t TAY(CPU6502 *cpu) {}
uint8_t TSX(CPU6502 *cpu) {}
uint8_t TXA(CPU6502 *cpu) {}
uint8_t TXS(CPU6502 *cpu) {}
uint8_t TYA(CPU6502 *cpu) {}
uint8_t XXX(CPU6502 *cpu) {}

void clock(CPU6502 *cpu) {
  if (cpu->cycles == 0) {
    Instructions instruction = LOOKUP[];
    cpu->cycles = instruction.cycles;
    uint8_t addrmode_additional_cycles = instruction.addrmode();
    uint8_t opmode_additional_cycles = instruction.opcode();
    cpu->cycles += addrmode_additional_cycles & opmode_additional_cycles;
  }

  cpu->cycles--;
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
  switch (opcode) {
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
