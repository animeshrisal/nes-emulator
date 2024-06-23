#include <stdint.h>

/// The NES has a 64 kb ram. So we create a ram array that is of 1 byte each
#define CPU_RAM_SIZE 64 * 1024

typedef struct {
  uint8_t memory[CPU_RAM_SIZE];
} Bus;

void write_to_bus(Bus *bus, uint16_t addr, uint8_t data);

uint8_t read_from_bus(Bus *bus, uint16_t addr);
