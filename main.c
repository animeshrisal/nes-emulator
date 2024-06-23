#include "./cartridge.h"
#include "./cpu.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  CPU6502 cpu;
  Bus bus;
  create_cpu(&cpu, &bus);
  printf("%d", cpu.bus);
  read_from_cartridge();
  return 0;
}
