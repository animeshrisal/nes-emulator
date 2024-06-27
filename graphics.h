#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

typedef struct {
  SDL_Renderer *renderer;
  SDL_Window *window;
} App;
