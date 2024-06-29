#include <stdint.h>

#include "./bus.h"
#include "mapper.h"

void write_to_bus(Bus *bus, uint16_t addr, uint8_t data) {
  if (addr >= 0x0000 && addr <= 0x0800) {
    bus->memory[addr] = data;
  };
}

uint8_t read_from_bus(Bus *bus, uint16_t addr) {
  if (addr >= 0x0000 && addr <= 0x0800) {
    return bus->memory[addr % 0x0800];
  };

  if (addr >= 0x8000) {
    return read_from_mapper(bus->mapper, addr);
  }

  return bus->memory[addr];
}

void hold_current_value(Bus *bus, uint16_t addr) {
  bus->current_value = read_from_bus(bus, addr);
};
