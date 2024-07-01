#include "cartridge.h"
#include "bus.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void load_cartridge(Bus *bus, Cartridge *cartridge) {
  FILE *file = fopen("nestest.nes", "rb");
  if (!file) {
    perror("Error opening file");
  };

  CartridgeHeader nesHeader;

  if (fread(&nesHeader, sizeof(CartridgeHeader), 1, file) != 1) {
    perror("Error reading from NES header file!");
    fclose(file);
  };

  if (nesHeader.signature[0] != 'N' || nesHeader.signature[1] != 'E' ||
      nesHeader.signature[2] != 'S' || nesHeader.signature[3] != '\x1A') {
    perror("Not a valid nes cartridge");
  }

  uint16_t prg_rom = nesHeader.prg_rom * PRG_ROM_SIZE;
  uint16_t chr_rom = nesHeader.chr_rom * CHR_ROM_SIZE;

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

uint16_t map_to_cartridge_address_range(Cartridge *cartridge, uint16_t addr) {
  return addr & (cartridge->cartridgeHeader.prg_rom > 1 ? 0x7FFF : 0x3FFF);
}
