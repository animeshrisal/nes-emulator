#include "cartridge.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void read_from_cartridge(Bus *bus, Cartridge *cartridge) {
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

  printf("0x%02x\n", nesHeader.mapper1);
  printf("%d\n", nesHeader.mapper2);
  cartridge->cartridgeHeader = nesHeader;
  cartridge->prgRomSize = malloc(prg_rom);
  cartridge->chrRomSize = malloc(chr_rom);

  if (fread(cartridge->prgRomSize, prg_rom, 1, file) != 1) {
    perror("Error reading prg!");
  }

  if (fread(cartridge->chrRomSize, chr_rom, 1, file) != 1) {
    perror("Error reading chr!");
  }

  for (int i = 0; i < prg_rom + 1; i++) {
    printf("0x%02X\n", cartridge->prgRomSize[i]);
  }

  fclose(file);
}
