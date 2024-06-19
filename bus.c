#include <stdint.h>

#include "./bus.h"

void write_to_bus(Bus *bus, uint16_t addr, uint8_t data) {
  if (addr >= 0x0000 && addr <= 0xFFFF) {
   bus->memory[addr] = data;
  };
}

uint8_t read_from_bus(Bus *bus, uint8_t addr) {
  return bus->memory[addr];
}

