#include "cpu.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

void initDisplay(SDL_Renderer *renderer, TTF_Font **font);

void display_message(SDL_Renderer *renderer);

void display_cpu_info(SDL_Renderer *renderer, CPU6502 *cpu, TTF_Font *font);
