#include "cpu.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

void display_text(SDL_Renderer *renderer, TTF_Font *font, const char *text,
                  int x, int y) {
  SDL_Color White = {255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(font, text, White);
  SDL_Texture *message = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  SDL_Rect message_rect = {x, y, 0, 0};
  SDL_QueryTexture(message, NULL, NULL, &message_rect.w, &message_rect.h);
  SDL_RenderCopy(renderer, message, NULL, &message_rect);
  SDL_DestroyTexture(message);
}

void initDisplay(SDL_Renderer *renderer, TTF_Font **font) {
  if (TTF_Init() != 0) {
    printf("Error loading ttf! %s\n", TTF_GetError());
  }

  *font = TTF_OpenFont("font.ttf", 38);
  if (*font == NULL) {
    printf("TTF_Font open error!");
  }
}

void display_register(SDL_Renderer *renderer, TTF_Font *font, const char *name,
                      uint8_t value, int x, int y) {
  char str[100];
  sprintf(str, "%s: %02X", name, value);
  display_text(renderer, font, str, x, y);
}

void display_flag(SDL_Renderer *renderer, TTF_Font *font, const char *name,
                  uint8_t value, int x, int y) {
  char str[100];
  sprintf(str, "%s: %d", name, value);
  display_text(renderer, font, str, x, y);
}

void display_cpu_info(SDL_Renderer *renderer, CPU6502 *cpu, TTF_Font *font) {
  int y = 10;

  display_register(renderer, font, "PC", cpu->PC, 10, y);
  y += 40;
  display_register(renderer, font, "A", cpu->A, 10, y);
  y += 40;
  display_register(renderer, font, "X", cpu->X, 10, y);
  y += 40;
  display_register(renderer, font, "Y", cpu->Y, 10, y);
  y += 40;
  display_register(renderer, font, "SP", cpu->SP, 10, y);
  y += 40;

  display_flag(renderer, font, "C", cpu->SR & C, 10, y);
  y += 40;
  display_flag(renderer, font, "Z", cpu->SR & Z, 10, y);
  y += 40;
  display_flag(renderer, font, "I", cpu->SR & I, 10, y);
  y += 40;
  display_flag(renderer, font, "D", cpu->SR & D, 10, y);
  y += 40;
  display_flag(renderer, font, "B", cpu->SR & B, 10, y);
  y += 40;
  display_flag(renderer, font, "U", cpu->SR & U, 10, y);
  y += 40;
  display_flag(renderer, font, "V", cpu->SR & V, 10, y);
  y += 40;
  display_flag(renderer, font, "N", cpu->SR & N, 10, y);
}
