#include "./mapper.h"
#include <stdbool.h>
#include <stdint.h>

bool read_from_mapper(Mapper *mapper, uint16_t addr, uint32_t mapped_addr) {
  if (addr >= 0x8000 && addr <= 0xFFFF) {
    mapped_addr = addr & (mapper->prg_banks > 1 ? 0x7FFF : 0x3FFF);
    return 1;
  }
  return 0;
}
