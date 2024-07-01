#include "./cpu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
  CPU6502 cpu;
  Bus bus;
  Cartridge cartridge;
  uint16_t startingPointer = 0x8000;

  create_cpu(&cpu, &bus);
  load_cartridge(&bus, &cartridge);
  reset_cpu(&cpu);

  /*
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error %s\n", SDL_GetError());
  };

  SDL_Window *window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 640, 480, 0);

  if (window == NULL) {
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  SDL_Delay(2000); // Wait for 2 seconds
  //
  */
  int test = 2000;
  while (test-- > 0) {
    onUpdate(&cpu);
  };

  // SDL_DestroyRenderer(renderer);
  // SDL_DestroyWindow(window);
  // SDL_Quit();

  return 0;
}

void initSDL() {}

void test() {}

void onCreate() {}
