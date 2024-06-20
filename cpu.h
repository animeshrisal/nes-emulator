#include "./bus.h"
#include <stdint.h>
#include <sys/types.h>

#define RESET_ADDRESS 0xFFFC

typedef struct {
  char name[4];
  uint8_t (*opcode)(void);
  uint8_t (*addrmode)(void);
  uint8_t cycles; 
} Instructions;

typedef struct {
  uint8_t A;
  uint8_t X;
  uint8_t Y;
  uint8_t SR; //Status register
  uint8_t SP; //Stack Pointer
  uint8_t PC; //Program counter
  Bus *bus;
  unsigned long cycles;
  uint8_t fetched; 
} CPU6502;

typedef enum {
    C = (1 << 0),   // Carry
    Z = (1 << 1),   // Zero
    I = (1 << 2),   // Interrupt Disable
    D = (1 << 3),   // Decimal (will not be used)
    B = (1 << 4),   // Break
    U = (1 << 5),   // Unused
    V = (1 << 6),   // Overflow
    N = (1 << 7),
} CPUStatusFlags;

void create_cpu(CPU6502 *cpu, Bus *bus);
uint8_t IMP();

uint8_t ADC();
uint8_t AND();
uint8_t ASL();
uint8_t BCC();
uint8_t BCS();
uint8_t BEQ();
uint8_t BIT();
uint8_t BMI();
uint8_t BNE();
uint8_t BPL();
uint8_t BRK();
uint8_t BVC();
uint8_t BVS();
uint8_t CLC();
uint8_t CLD();
uint8_t CLI();
uint8_t CLV();
uint8_t CMP();
uint8_t CPX();
uint8_t CPY();
uint8_t DEC();
uint8_t DEX();
uint8_t DEY();
uint8_t EOR();
uint8_t INC();
uint8_t INX();
uint8_t INY();
uint8_t JMP();
uint8_t JSR();
uint8_t LDA();
uint8_t LDX();
uint8_t LDY();
uint8_t LSR();
uint8_t NOP();
uint8_t ORA();
uint8_t PHA();
uint8_t PHP();
uint8_t PLA();
uint8_t PLP();
uint8_t ROL();
uint8_t ROR();
uint8_t RTI();
uint8_t RTS();
uint8_t SBC();
uint8_t SEC();
uint8_t SED();
uint8_t SEI();
uint8_t STA();
uint8_t STX();
uint8_t STY();
uint8_t TAX();
uint8_t TAY();
uint8_t TSX();
uint8_t TXA();
uint8_t TXS();
uint8_t TYA();
uint8_t XXX();

uint8_t IMM(); // Immediate
uint8_t ZP0(); // Zero Page
uint8_t ZPX(); // Zero Page,X
uint8_t ZPY(); // Zero Page,Y
uint8_t REL(); // Relative
uint8_t ABS(); // Absolute
uint8_t ABX(); // Absolute,X
uint8_t ABY(); // Absolute,Y
uint8_t IND(); // Indirect
uint8_t IZX(); // (Indirect,X)
uint8_t IZY(); // (Indirect),Y
