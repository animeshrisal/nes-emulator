#include "./bus.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void write_to_memory(Bus *bus, uint16_t addr, uint8_t data) {
  bus->memory[addr] = data;
}

uint8_t read_from_memory(Bus *bus, uint16_t addr) {
  if (addr >= 0x0000 && addr <= 0x0800) {
    return bus->memory[addr % 0x0800];
  };

  if (addr >= 0x8000) {
    return read_from_cartridge(bus, addr);
  }

  return bus->memory[addr];
}

void load_cartridge(Bus *bus, Cartridge *cartridge) {
  FILE *file = fopen("basics.nes", "rb");
  if (!file) {
    perror("Error opening file");
  };

  CartridgeHeader nesHeader;

  if (fread(&nesHeader, sizeof(CartridgeHeader), 1, file) != 1) {
    perror("Error reading from NES header file!");
    fclose(file);
  }

  if (nesHeader.signature[0] != 'N' || nesHeader.signature[1] != 'E' ||
      nesHeader.signature[2] != 'S' || nesHeader.signature[3] != '\x1A') {
    perror("Not a valid nes cartridge");
  }

  uint16_t prg_rom = nesHeader.prg_rom * PRG_ROM_SIZE;
  uint16_t chr_rom = nesHeader.chr_rom * CHR_ROM_SIZE;

  printf("%d\n", prg_rom);
  printf("%d\n", nesHeader.chr_rom);
  cartridge->cartridgeHeader = nesHeader;

  cartridge->prgRomSize = malloc(prg_rom);

  cartridge->chrRomSize = malloc(chr_rom);

  if (fread(cartridge->prgRomSize, prg_rom, 1, file) != 1) {
    perror("Error reading prg!");
  }

  if (fread(cartridge->chrRomSize, chr_rom, 1, file) != 1) {
    perror("Error reading chr!");
  }

  fclose(file);

  bus->cartridge = cartridge;
}

uint16_t read_from_cartridge(Bus *bus, uint16_t addr) {
  return map_to_cartridge_address_range(bus->cartridge, addr);
}

uint8_t map_to_cartridge_address_range(Cartridge *cartridge, uint16_t addr) {
  uint16_t mapped_addr =
      addr & (cartridge->cartridgeHeader.prg_rom > 1 ? 0x7FFF : 0x3FFF);
  return cartridge->prgRomSize[mapped_addr];
}
