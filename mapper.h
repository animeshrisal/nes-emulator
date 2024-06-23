#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t prg_banks;
  uint8_t chr_banks;
} Mapper;

bool read_from_mapper(Mapper *mapper, uint16_t addr, uint32_t mapped_addr);
