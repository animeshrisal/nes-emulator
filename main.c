#include "./cpu.h"
#include "./viewer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <stdint.h>

#define DISPLAY_TEXT_SIZE 10

int main(int argc, char *argv[]) {
  CPU6502 cpu;
  Bus bus;
  Cartridge cartridge;
  uint16_t startingPointer = 0x8000;
  TTF_Font *font;
  char code[0xFFFF][100];

  create_cpu(&cpu, &bus);
  load_cartridge(&bus, &cartridge);
  prepare_code(code, &cpu);
  reset_cpu(&cpu);

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL_Init Error %s\n", SDL_GetError());
  };

  SDL_Window *window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, 1200, 800, 0);

  if (window == NULL) {
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Color White = {255, 255, 255};

  SDL_Rect message_rect;
  message_rect.x = 0;
  message_rect.y = 0;
  message_rect.w = 160;
  message_rect.h = 90;

  char str[100];
  int test = 20000;
  SDL_Surface *surfaceMessage;
  SDL_Texture *message;

  initDisplay(renderer, &font);
  int quit = 0;
  SDL_Event event;
  display_cpu_info(renderer, &cpu, font);
  display_instructions(renderer, &cpu, font, code);
  SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
  SDL_RenderPresent(renderer);

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_KEYDOWN:
        do {
          onUpdate(&cpu);
          display_cpu_info(renderer, &cpu, font);
          display_instructions(renderer, &cpu, font, code);
          SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
          SDL_RenderPresent(renderer);

          SDL_RenderClear(renderer);
        } while (cpu.cycles == 0);

        do {
          onUpdate(&cpu);
          display_cpu_info(renderer, &cpu, font);
          display_instructions(renderer, &cpu, font, code);
          SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
          SDL_RenderPresent(renderer);

          SDL_RenderClear(renderer);
        } while (cpu.cycles != 0);

        break;
      case SDL_QUIT:
        quit = 1;
        break;
      }
    }
  }

  /*
  while (1) {
    onUpdate(&cpu);
    display_cpu_info(renderer, &cpu, font);
    display_instructions(renderer, &cpu, font, code);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 0);
    SDL_RenderPresent(renderer);

    SDL_RenderClear(renderer);
  };
  */

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

void initSDL() {}

void test() {}

void onCreate() {}
