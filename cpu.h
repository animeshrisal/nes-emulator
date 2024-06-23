#include "./bus.h"
#include <stdint.h>
#include <sys/types.h>

#define RESET_ADDRESS 0xFFFC

typedef struct {
  uint8_t A;
  uint8_t X;
  uint8_t Y;
  uint8_t SR; // Status regisjter
  uint8_t SP; // Stack Pointer
  uint8_t PC; // Program counter
  Bus *bus;
  unsigned long cycles;
  uint8_t fetched;
} CPU6502;

typedef struct {
  char name[3];
  uint8_t (*opcode)(CPU6502 *cpu);
  uint8_t (*addrmode)(CPU6502 *cpu);
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
uint8_t IMP(CPU6502 *cpu);

uint8_t ADC(CPU6502 *cpu);
uint8_t AND(CPU6502 *cpu);
uint8_t ASL(CPU6502 *cpu);
uint8_t BCC(CPU6502 *cpu);
uint8_t BCS(CPU6502 *cpu);
uint8_t BEQ(CPU6502 *cpu);
uint8_t BIT(CPU6502 *cpu);
uint8_t BMI(CPU6502 *cpu);
uint8_t BNE(CPU6502 *cpu);
uint8_t BPL(CPU6502 *cpu);
uint8_t BRK(CPU6502 *cpu);
uint8_t BVC(CPU6502 *cpu);
uint8_t BVS(CPU6502 *cpu);
uint8_t CLC(CPU6502 *cpu);
uint8_t CLD(CPU6502 *cpu);
uint8_t CLI(CPU6502 *cpu);
uint8_t CLV(CPU6502 *cpu);
uint8_t CMP(CPU6502 *cpu);
uint8_t CPX(CPU6502 *cpu);
uint8_t CPY(CPU6502 *cpu);
uint8_t DEC(CPU6502 *cpu);
uint8_t DEX(CPU6502 *cpu);
uint8_t DEY(CPU6502 *cpu);
uint8_t EOR(CPU6502 *cpu);
uint8_t INC(CPU6502 *cpu);
uint8_t INX(CPU6502 *cpu);
uint8_t INY(CPU6502 *cpu);
uint8_t JMP(CPU6502 *cpu);
uint8_t JSR(CPU6502 *cpu);
uint8_t LDA(CPU6502 *cpu);
uint8_t LDX(CPU6502 *cpu);
uint8_t LDY(CPU6502 *cpu);
uint8_t LSR(CPU6502 *cpu);
uint8_t NOP(CPU6502 *cpu);
uint8_t ORA(CPU6502 *cpu);
uint8_t PHA(CPU6502 *cpu);
uint8_t PHP(CPU6502 *cpu);
uint8_t PLA(CPU6502 *cpu);
uint8_t PLP(CPU6502 *cpu);
uint8_t ROL(CPU6502 *cpu);
uint8_t ROR(CPU6502 *cpu);
uint8_t RTI(CPU6502 *cpu);
uint8_t RTS(CPU6502 *cpu);
uint8_t SBC(CPU6502 *cpu);
uint8_t SEC(CPU6502 *cpu);
uint8_t SED(CPU6502 *cpu);
uint8_t SEI(CPU6502 *cpu);
uint8_t STA(CPU6502 *cpu);
uint8_t STX(CPU6502 *cpu);
uint8_t STY(CPU6502 *cpu);
uint8_t TAX(CPU6502 *cpu);
uint8_t TAY(CPU6502 *cpu);
uint8_t TSX(CPU6502 *cpu);
uint8_t TXA(CPU6502 *cpu);
uint8_t TXS(CPU6502 *cpu);
uint8_t TYA(CPU6502 *cpu);
uint8_t XXX(CPU6502 *cpu);

uint8_t IMM(CPU6502 *cpu);
uint8_t IMP(CPU6502 *cpu); // Immediate
uint8_t ZP0(CPU6502 *cpu); // Zero Page
uint8_t ZPX(CPU6502 *cpu); // Zero Page,X
uint8_t ZPY(CPU6502 *cpu); // Zero Page,Y
uint8_t REL(CPU6502 *cpu); // Relative
uint8_t ABS(CPU6502 *cpu); // Absolute
uint8_t ABX(CPU6502 *cpu); // Absolute,X
uint8_t ABY(CPU6502 *cpu); // Absolute,Y
uint8_t IND(CPU6502 *cpu); // Indirect
uint8_t IZX(CPU6502 *cpu); // (Indirect,X)
uint8_t IZY(CPU6502 *cpu); // (Indirect),Y
