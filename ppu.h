#include "cartridge.h"

typedef struct Color {
  uint8_t R;
  uint8_t G;
  uint8_t B;
} Color;

typedef struct {
  Color *pixels;
  uint16_t width;
  uint16_t height;
} Sprite;

typedef union PpuCtrl {
  struct {
    uint8_t nametable_x : 1;
    uint8_t nametable_y : 1;
    uint8_t increment_mode : 1;
    uint8_t pattern_sprite : 1;
    uint8_t pattern_background : 1;
    uint8_t sprite_size : 1;
    uint8_t slave_mode : 1;
    uint8_t enable_nmi : 1;
  } bits;

  uint8_t reg;
} PpuCtrl;

typedef union PpuStatus {
  struct {
    uint8_t unused : 5;
    uint8_t sprite_overflow : 1;
    uint8_t sprize_zero_hit : 1;
    uint8_t vertical_blank : 1;
  };
} PpuStatus;

typedef union PpuMask {
  struct {
    uint8_t grayscale : 1;
    uint8_t render_background_left : 1;
    uint8_t render_sprites_left : 1;
    uint8_t render_background : 1;
    uint8_t render_sprites : 1;
    uint8_t enhance_red : 1;
    uint8_t enhance_green : 1;
    uint8_t enhance_blue : 1;
  };

  uint8_t reg;
} PpuMask;

typedef struct PpuRegisters {
  PpuCtrl ctrl;
  PpuStatus status;
  PpuMask mask;
} PpuRegisters;

typedef union LoopyRegister {
  struct {
    uint16_t coarse_x : 5;
    uint16_t coarse_y : 5;
    uint16_t nametable_x : 1;
    uint16_t nametable_y : 1;
    uint16_t fine_y : 3;
    uint16_t unused : 1;
  };

  uint16_t reg = 0x0000;
} LoopyRegister;

void PpuInit();

uint8_t cpu_read_from_ppu(uint16_t addr, int read_only);
void cpu_write_to_ppy(uint16_t addr, uint8_t data);

uint8_t ppu_read(uint16_t addr);
void ppu_write(uint16_t addr, uint8_t data);

void ppu_connect_cartridge(Cartridge *cartridge);
void ppu_clock();
