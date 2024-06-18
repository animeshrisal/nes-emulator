#include <SDL2/SDL.h>
#include <stdio.h>
#include "./cpu.h"

int main(int argc, char* argv[]) {

  CPU6502 cpu;
  Bus bus;
  create_cpu(&cpu, &bus);
  
  printf("%d", cpu.&bus);
  return 0;
}


