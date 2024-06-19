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
  uint16_t PC; //Program counter
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
