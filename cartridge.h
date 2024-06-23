#include <stdint.h>

#define HEADER_SIZE 4
#define UNUSED_SIZE 5

typedef struct {
  uint8_t signature[HEADER_SIZE];
  uint8_t prg_rom;
  uint8_t chr_rom;
  uint8_t mapper1;
  uint8_t mapper2;
  uint8_t prg_ram_size;
  uint8_t tv_system_1;
  uint8_t tv_system_2;
  uint8_t unused_space[UNUSED_SIZE];
} CartridgeHeader;

void read_from_cartridge();
