#ifndef BUS_H
#define BUS_H

#include "./cartridge.h"
#include <stdint.h>
/// The NES has a 64 kb ram. So we create a ram array that is of 1 byte each
#define CPU_RAM_SIZE 64 * 1024

typedef struct {
  uint8_t memory[CPU_RAM_SIZE];
  Cartridge *cartridge;
  uint8_t current_value;
} Bus;

void write_to_memory(Bus *bus, uint16_t addr, uint8_t data);

uint8_t read_from_memory(Bus *bus, uint16_t addr);

void hold_current_value(Bus *bus, uint16_t addr);

void load_cartridge(Bus *bus, Cartridge *cartridge);

void read_from_cartridge(Bus *bus);
#endif
