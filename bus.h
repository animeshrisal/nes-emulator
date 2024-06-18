#include<stdint.h>


#define CPU_RAM_SIZE 64 * 1024


typedef struct {
 uint8_t memory[CPU_RAM_SIZE];
} Bus;

