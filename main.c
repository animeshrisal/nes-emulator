#include "./cartridge.h"
#include "./cpu.h"
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  CPU6502 cpu;
  Bus bus;
  Cartridge cartridge;

  create_cpu(&cpu, &bus);
  read_from_cartridge(&cartridge);
  return 0;
}
