#include "./cartridge.h"
#include "./cpu.h"
#include <SDL2/SDL.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
  CPU6502 cpu;
  Bus bus;
  Cartridge cartridge;
  uint16_t startingPointer = 0x8000;

  create_cpu(&cpu, &bus);
  read_from_cartridge(&bus, &cartridge);

  return 0;
}

void test() {}
