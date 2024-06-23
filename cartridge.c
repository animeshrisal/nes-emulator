#include "cartridge.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void read_from_cartridge(Cartridge *cartridge) {
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
  printf("%d\n", nesHeader.chr_rom);
  printf("%d\n", nesHeader.prg_rom);
  printf("Is a valid nes cartridge!");

  uint16_t prg_rom = nesHeader.prg_rom * PRG_ROM_SIZE;
  uint16_t chr_rom = nesHeader.chr_rom * CHR_ROM_SIZE;

  cartridge->cartridgeHeader = nesHeader;
  cartridge->prgRomSize =
      (uint8_t *)malloc(nesHeader.prg_rom * sizeof(uint8_t));
  cartridge->chrRomSize =
      (uint8_t *)malloc(nesHeader.prg_rom * sizeof(uint8_t));

  if (fread(cartridge->prgRomSize, prg_rom, 1, file) != 1) {
    perror("Error reading prg!");
  }

  for (int i = 0; i < prg_rom; i++) {
    printf("0x%02X\n", cartridge->prgRomSize[i]);
  }
}
