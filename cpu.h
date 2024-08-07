#ifndef CPU_H
#define CPU_H

#include "./bus.h"
#include <stdint.h>
#include <sys/types.h>

#define RESET_ADDRESS 0xFFFC

typedef struct {
  uint8_t A;
  uint8_t X;
  uint8_t Y;
  uint8_t SR;  // Status regisjter
  uint8_t SP;  // Stack Pointer
  uint16_t PC; // Program counter
  Bus *bus;
  unsigned long cycles;
  uint16_t (*addrmode)();
} CPU6502;

typedef struct {
  char name[4];
  uint8_t (*opcode)(CPU6502 *cpu, uint16_t addr);
  uint16_t (*addrmode)(CPU6502 *cpu);
  uint8_t cycles;
} Instructions;

typedef enum {
  C = (1 << 0), // Carry
  Z = (1 << 1), // Zero
  I = (1 << 2), // Interrupt Disable
  D = (1 << 3), // Decimal (will not be used)
  B = (1 << 4), // Break
  U = (1 << 5), // Unused
  V = (1 << 6), // Overflow
  N = (1 << 7),
} CPUStatusFlags;

void create_cpu(CPU6502 *cpu, Bus *bus);
void reset_cpu(CPU6502 *cpu);
void hold_current_value(CPU6502 *cpu, uint16_t addr);

uint8_t ADC(CPU6502 *cpu, uint16_t addr);
uint8_t AND(CPU6502 *cpu, uint16_t addr);
uint8_t ASL(CPU6502 *cpu, uint16_t addr);
uint8_t BCC(CPU6502 *cpu, uint16_t addr);
uint8_t BCS(CPU6502 *cpu, uint16_t addr);
uint8_t BEQ(CPU6502 *cpu, uint16_t addr);
uint8_t BIT(CPU6502 *cpu, uint16_t addr);
uint8_t BMI(CPU6502 *cpu, uint16_t addr);
uint8_t BNE(CPU6502 *cpu, uint16_t addr);
uint8_t BPL(CPU6502 *cpu, uint16_t addr);
uint8_t BRK(CPU6502 *cpu, uint16_t addr);
uint8_t BVC(CPU6502 *cpu, uint16_t addr);
uint8_t BVS(CPU6502 *cpu, uint16_t addr);
uint8_t CLC(CPU6502 *cpu, uint16_t addr);
uint8_t CLD(CPU6502 *cpu, uint16_t addr);
uint8_t CLI(CPU6502 *cpu, uint16_t addr);
uint8_t CLV(CPU6502 *cpu, uint16_t addr);
uint8_t CMP(CPU6502 *cpu, uint16_t addr);
uint8_t CPX(CPU6502 *cpu, uint16_t addr);
uint8_t CPY(CPU6502 *cpu, uint16_t addr);
uint8_t DEC(CPU6502 *cpu, uint16_t addr);
uint8_t DEX(CPU6502 *cpu, uint16_t addr);
uint8_t DEY(CPU6502 *cpu, uint16_t addr);
uint8_t EOR(CPU6502 *cpu, uint16_t addr);
uint8_t INC(CPU6502 *cpu, uint16_t addr);
uint8_t INX(CPU6502 *cpu, uint16_t addr);
uint8_t INY(CPU6502 *cpu, uint16_t addr);
uint8_t JMP(CPU6502 *cpu, uint16_t addr);
uint8_t JSR(CPU6502 *cpu, uint16_t addr);
uint8_t LDA(CPU6502 *cpu, uint16_t addr);
uint8_t LDX(CPU6502 *cpu, uint16_t addr);
uint8_t LDY(CPU6502 *cpu, uint16_t addr);
uint8_t LSR(CPU6502 *cpu, uint16_t addr);
uint8_t NOP(CPU6502 *cpu, uint16_t addr);
uint8_t ORA(CPU6502 *cpu, uint16_t addr);
uint8_t PHA(CPU6502 *cpu, uint16_t addr);
uint8_t PHP(CPU6502 *cpu, uint16_t addr);
uint8_t PLA(CPU6502 *cpu, uint16_t addr);
uint8_t PLP(CPU6502 *cpu, uint16_t addr);
uint8_t ROL(CPU6502 *cpu, uint16_t addr);
uint8_t ROR(CPU6502 *cpu, uint16_t addr);
uint8_t RTI(CPU6502 *cpu, uint16_t addr);
uint8_t RTS(CPU6502 *cpu, uint16_t addr);
uint8_t SBC(CPU6502 *cpu, uint16_t addr);
uint8_t SEC(CPU6502 *cpu, uint16_t addr);
uint8_t SED(CPU6502 *cpu, uint16_t addr);
uint8_t SEI(CPU6502 *cpu, uint16_t addr);
uint8_t STA(CPU6502 *cpu, uint16_t addr);
uint8_t STX(CPU6502 *cpu, uint16_t addr);
uint8_t STY(CPU6502 *cpu, uint16_t addr);
uint8_t TAX(CPU6502 *cpu, uint16_t addr);
uint8_t TAY(CPU6502 *cpu, uint16_t addr);
uint8_t TSX(CPU6502 *cpu, uint16_t addr);
uint8_t TXA(CPU6502 *cpu, uint16_t addr);
uint8_t TXS(CPU6502 *cpu, uint16_t addr);
uint8_t TYA(CPU6502 *cpu, uint16_t addr);
uint8_t XXX(CPU6502 *cpu, uint16_t addr);

uint16_t IMM(CPU6502 *cpu); // Immediate
uint16_t IMP(CPU6502 *cpu); // Implied
uint16_t ZP0(CPU6502 *cpu); // Zero Page
uint16_t ZPX(CPU6502 *cpu); // Zero Page,X
uint16_t ZPY(CPU6502 *cpu); // Zero Page,Y
uint16_t REL(CPU6502 *cpu); // Relative
uint16_t ABS(CPU6502 *cpu); // Absolute
uint16_t ABX(CPU6502 *cpu); // Absolute,X
uint16_t ABY(CPU6502 *cpu); // Absolute,Y
uint16_t IND(CPU6502 *cpu); // Indirect
uint16_t IZX(CPU6502 *cpu); // (Indirect,X)
uint16_t IZY(CPU6502 *cpu); // (Indirect),Y

void onUpdate(CPU6502 *cpu);

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

uint8_t get_opcode(Bus *bus, uint16_t addr);
void prepare_code(char (*code)[100], CPU6502 *cpu);

#endif
