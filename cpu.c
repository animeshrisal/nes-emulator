#include "./cpu.h"
#include "bus.h"
#include <stdint.h>

static Instructions LOOKUP[16 * 16] = {
    {"BRK", BRK, IMP, 7}, {"ORA", ORA, IZX, 6}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 8}, {"???", NOP, IMP, 3}, {"ORA", ORA, ZP0, 3},
    {"ASL", ASL, ZP0, 5}, {"???", XXX, IMP, 5}, {"PHP", PHP, IMP, 3},
    {"ORA", ORA, IMM, 2}, {"ASL", ASL, IMP, 2}, {"???", XXX, IMP, 2},
    {"???", NOP, IMP, 4}, {"ORA", ORA, ABS, 4}, {"ASL", ASL, ABS, 6},
    {"???", XXX, IMP, 6}, {"BPL", BPL, REL, 2}, {"ORA", ORA, IZY, 5},
    {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4},
    {"ORA", ORA, ZPX, 4}, {"ASL", ASL, ZPX, 6}, {"???", XXX, IMP, 6},
    {"CLC", CLC, IMP, 2}, {"ORA", ORA, ABY, 4}, {"???", NOP, IMP, 2},
    {"???", XXX, IMP, 7}, {"???", NOP, IMP, 4}, {"ORA", ORA, ABX, 4},
    {"ASL", ASL, ABX, 7}, {"???", XXX, IMP, 7}, {"JSR", JSR, ABS, 6},
    {"AND", AND, IZX, 6}, {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8},
    {"BIT", BIT, ZP0, 3}, {"AND", AND, ZP0, 3}, {"ROL", ROL, ZP0, 5},
    {"???", XXX, IMP, 5}, {"PLP", PLP, IMP, 4}, {"AND", AND, IMM, 2},
    {"ROL", ROL, IMP, 2}, {"???", XXX, IMP, 2}, {"BIT", BIT, ABS, 4},
    {"AND", AND, ABS, 4}, {"ROL", ROL, ABS, 6}, {"???", XXX, IMP, 6},
    {"BMI", BMI, REL, 2}, {"AND", AND, IZY, 5}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4}, {"AND", AND, ZPX, 4},
    {"ROL", ROL, ZPX, 6}, {"???", XXX, IMP, 6}, {"SEC", SEC, IMP, 2},
    {"AND", AND, ABY, 4}, {"???", NOP, IMP, 2}, {"???", XXX, IMP, 7},
    {"???", NOP, IMP, 4}, {"AND", AND, ABX, 4}, {"ROL", ROL, ABX, 7},
    {"???", XXX, IMP, 7}, {"RTI", RTI, IMP, 6}, {"EOR", EOR, IZX, 6},
    {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8}, {"???", NOP, IMP, 3},
    {"EOR", EOR, ZP0, 3}, {"LSR", LSR, ZP0, 5}, {"???", XXX, IMP, 5},
    {"PHA", PHA, IMP, 3}, {"EOR", EOR, IMM, 2}, {"LSR", LSR, IMP, 2},
    {"???", XXX, IMP, 2}, {"JMP", JMP, ABS, 3}, {"EOR", EOR, ABS, 4},
    {"LSR", LSR, ABS, 6}, {"???", XXX, IMP, 6}, {"BVC", BVC, REL, 2},
    {"EOR", EOR, IZY, 5}, {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8},
    {"???", NOP, IMP, 4}, {"EOR", EOR, ZPX, 4}, {"LSR", LSR, ZPX, 6},
    {"???", XXX, IMP, 6}, {"CLI", CLI, IMP, 2}, {"EOR", EOR, ABY, 4},
    {"???", NOP, IMP, 2}, {"???", XXX, IMP, 7}, {"???", NOP, IMP, 4},
    {"EOR", EOR, ABX, 4}, {"LSR", LSR, ABX, 7}, {"???", XXX, IMP, 7},
    {"RTS", RTS, IMP, 6}, {"ADC", ADC, IZX, 6}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 8}, {"???", NOP, IMP, 3}, {"ADC", ADC, ZP0, 3},
    {"ROR", ROR, ZP0, 5}, {"???", XXX, IMP, 5}, {"PLA", PLA, IMP, 4},
    {"ADC", ADC, IMM, 2}, {"ROR", ROR, IMP, 2}, {"???", XXX, IMP, 2},
    {"JMP", JMP, IND, 5}, {"ADC", ADC, ABS, 4}, {"ROR", ROR, ABS, 6},
    {"???", XXX, IMP, 6}, {"BVS", BVS, REL, 2}, {"ADC", ADC, IZY, 5},
    {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4},
    {"ADC", ADC, ZPX, 4}, {"ROR", ROR, ZPX, 6}, {"???", XXX, IMP, 6},
    {"SEI", SEI, IMP, 2}, {"ADC", ADC, ABY, 4}, {"???", NOP, IMP, 2},
    {"???", XXX, IMP, 7}, {"???", NOP, IMP, 4}, {"ADC", ADC, ABX, 4},
    {"ROR", ROR, ABX, 7}, {"???", XXX, IMP, 7}, {"???", NOP, IMP, 2},
    {"STA", STA, IZX, 6}, {"???", NOP, IMP, 2}, {"???", XXX, IMP, 6},
    {"STY", STY, ZP0, 3}, {"STA", STA, ZP0, 3}, {"STX", STX, ZP0, 3},
    {"???", XXX, IMP, 3}, {"DEY", DEY, IMP, 2}, {"???", NOP, IMP, 2},
    {"TXA", TXA, IMP, 2}, {"???", XXX, IMP, 2}, {"STY", STY, ABS, 4},
    {"STA", STA, ABS, 4}, {"STX", STX, ABS, 4}, {"???", XXX, IMP, 4},
    {"BCC", BCC, REL, 2}, {"STA", STA, IZY, 6}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 6}, {"STY", STY, ZPX, 4}, {"STA", STA, ZPX, 4},
    {"STX", STX, ZPY, 4}, {"???", XXX, IMP, 4}, {"TYA", TYA, IMP, 2},
    {"STA", STA, ABY, 5}, {"TXS", TXS, IMP, 2}, {"???", XXX, IMP, 5},
    {"???", NOP, IMP, 5}, {"STA", STA, ABX, 5}, {"???", XXX, IMP, 5},
    {"???", XXX, IMP, 5}, {"LDY", LDY, IMM, 2}, {"LDA", LDA, IZX, 6},
    {"LDX", LDX, IMM, 2}, {"???", XXX, IMP, 6}, {"LDY", LDY, ZP0, 3},
    {"LDA", LDA, ZP0, 3}, {"LDX", LDX, ZP0, 3}, {"???", XXX, IMP, 3},
    {"TAY", TAY, IMP, 2}, {"LDA", LDA, IMM, 2}, {"TAX", TAX, IMP, 2},
    {"???", XXX, IMP, 2}, {"LDY", LDY, ABS, 4}, {"LDA", LDA, ABS, 4},
    {"LDX", LDX, ABS, 4}, {"???", XXX, IMP, 4}, {"BCS", BCS, REL, 2},
    {"LDA", LDA, IZY, 5}, {"???", XXX, IMP, 2}, {"???", XXX, IMP, 5},
    {"LDY", LDY, ZPX, 4}, {"LDA", LDA, ZPX, 4}, {"LDX", LDX, ZPY, 4},
    {"???", XXX, IMP, 4}, {"CLV", CLV, IMP, 2}, {"LDA", LDA, ABY, 4},
    {"TSX", TSX, IMP, 2}, {"???", XXX, IMP, 4}, {"LDY", LDY, ABX, 4},
    {"LDA", LDA, ABX, 4}, {"LDX", LDX, ABY, 4}, {"???", XXX, IMP, 4},
    {"CPY", CPY, IMM, 2}, {"CMP", CMP, IZX, 6}, {"???", NOP, IMP, 2},
    {"???", XXX, IMP, 8}, {"CPY", CPY, ZP0, 3}, {"CMP", CMP, ZP0, 3},
    {"DEC", DEC, ZP0, 5}, {"???", XXX, IMP, 5}, {"INY", INY, IMP, 2},
    {"CMP", CMP, IMM, 2}, {"DEX", DEX, IMP, 2}, {"???", XXX, IMP, 2},
    {"CPY", CPY, ABS, 4}, {"CMP", CMP, ABS, 4}, {"DEC", DEC, ABS, 6},
    {"???", XXX, IMP, 6}, {"BNE", BNE, REL, 2}, {"CMP", CMP, IZY, 5},
    {"???", XXX, IMP, 2}, {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4},
    {"CMP", CMP, ZPX, 4}, {"DEC", DEC, ZPX, 6}, {"???", XXX, IMP, 6},
    {"CLD", CLD, IMP, 2}, {"CMP", CMP, ABY, 4}, {"NOP", NOP, IMP, 2},
    {"???", XXX, IMP, 7}, {"???", NOP, IMP, 4}, {"CMP", CMP, ABX, 4},
    {"DEC", DEC, ABX, 7}, {"???", XXX, IMP, 7}, {"CPX", CPX, IMM, 2},
    {"SBC", SBC, IZX, 6}, {"???", NOP, IMP, 2}, {"???", XXX, IMP, 8},
    {"CPX", CPX, ZP0, 3}, {"SBC", SBC, ZP0, 3}, {"INC", INC, ZP0, 5},
    {"???", XXX, IMP, 5}, {"INX", INX, IMP, 2}, {"SBC", SBC, IMM, 2},
    {"NOP", NOP, IMP, 2}, {"???", SBC, IMP, 2}, {"CPX", CPX, ABS, 4},
    {"SBC", SBC, ABS, 4}, {"INC", INC, ABS, 6}, {"???", XXX, IMP, 6},
    {"BEQ", BEQ, REL, 2}, {"SBC", SBC, IZY, 5}, {"???", XXX, IMP, 2},
    {"???", XXX, IMP, 8}, {"???", NOP, IMP, 4}, {"SBC", SBC, ZPX, 4},
    {"INC", INC, ZPX, 6}, {"???", XXX, IMP, 6}, {"SED", SED, IMP, 2},
    {"SBC", SBC, ABY, 4}, {"NOP", NOP, IMP, 2}, {"???", XXX, IMP, 7},
    {"???", NOP, IMP, 4}, {"SBC", SBC, ABX, 4}, {"INC", INC, ABX, 7},
    {"???", XXX, IMP, 7},
};

void create_cpu(CPU6502 *cpu, Bus *bus) {
  cpu->A = 0;
  cpu->X = 0;
  cpu->Y = 0;
  cpu->PC = 0;
  cpu->SP = 0xfd;

  cpu->cycles = 7;
  cpu->bus = bus;
}

uint8_t get_opcode(Bus *bus, uint16_t addr) { return 1; }

int get_flag(CPU6502 *cpu, CPUStatusFlags flag) {
  return (cpu->SR >> flag) & 0x1;
}

void set_flag(CPU6502 *cpu, CPUStatusFlags flag, int bools) {
  if (bools) {
    cpu->SR |= 0x1 << flag;
  } else {
    cpu->SR &= ~(0x1 << flag);
  }
}

void instruction_lookup() {}

/*
void reset(CPU6502 *cpu) {
  uint16_t abs_addr = 0xFFFC;
  uint16_t low = read_address(abs_addr);
  uint16_t high = read_address(abs_addr + 1);

  cpu->PC = (high << 8) | low;
};*/

/*
 * Addressing Mode
 */

/*
 * Instructions
 */

uint8_t ADC(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  uint16_t temp = (uint16_t)cpu->A + (uint16_t)cpu->bus->current_value +
                  (uint16_t)get_flag(cpu, C);

  set_flag(cpu, C, temp > 255);
  set_flag(cpu, V,
           (~((uint16_t)cpu->A ^ (uint16_t)cpu->bus->current_value) &
            ((uint16_t)cpu->A ^ (uint16_t)temp)) &
               0x0080);

  set_flag(cpu, N, temp & 0x80);

  cpu->A = temp & 0x00FF;
  return 0;
}

uint8_t AND(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);

  uint8_t value = cpu->bus->current_value;
  cpu->A &= value;

  set_flag(cpu, C, cpu->A > 255);
  set_flag(cpu, Z, (cpu->A & 0x00FF) == 0);

  return 0;
}

uint8_t ASL(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  uint16_t temp = cpu->bus->current_value << 1;
  return 0;
}

uint8_t BCC(CPU6502 *cpu) {
  if (get_flag(cpu, B) == 0) {
    uint16_t addr_abs = 0xff00;
    cpu->cycles++;

    cpu->PC = addr_abs;
  }
  return 0;
}

uint8_t BCS(CPU6502 *cpu) {

  if (get_flag(cpu, C) == 1) {
    uint16_t addr_abs = 0xff00;
    cpu->cycles++;

    cpu->PC = addr_abs;
  }

  return 0;
}

uint8_t BEQ(CPU6502 *cpu) {

  if (get_flag(cpu, Z) == 1) {
    uint16_t addr_abs = 0xff00;
    cpu->cycles++;

    cpu->PC = addr_abs;
  }

  return 0;
}

uint8_t BIT(CPU6502 *cpu) { return 0; }

uint8_t BMI(CPU6502 *cpu) {

  if (get_flag(cpu, N) == 1) {
    uint16_t addr_abs = 0xff00;
    cpu->cycles++;

    cpu->PC = addr_abs;
  }

  return 0;
}

uint8_t BNE(CPU6502 *cpu) {
  if (get_flag(cpu, Z) == 0) {
    uint16_t addr_abs = 0xff00;
    cpu->cycles++;

    cpu->PC = addr_abs;
  }

  return 0;
}
uint8_t BPL(CPU6502 *cpu) {
  if (get_flag(cpu, N) == 0) {
    uint16_t addr_abs = 0xff00;
    cpu->cycles++;

    cpu->PC = addr_abs;
  }
  return 0;
}
uint8_t BRK(CPU6502 *cpu) { return 0; }
uint8_t BVC(CPU6502 *cpu) {
  if (get_flag(cpu, V) == 0) {
    uint16_t addr_abs = 0xff00;
    cpu->cycles++;

    cpu->PC = addr_abs;
  }
  return 0;
}

uint8_t BVS(CPU6502 *cpu) {
  if (get_flag(cpu, V) == 0) {
    uint16_t addr_abs = 0xff00;
    cpu->cycles++;

    cpu->PC = addr_abs;
  }
  return 0;
}

// clear carry flag
uint8_t CLC(CPU6502 *cpu) {
  set_flag(cpu, C, false);
  return 0;
};

uint8_t CLD(CPU6502 *cpu) {
  set_flag(cpu, D, false);
  return 0;
}

uint8_t CLI(CPU6502 *cpu) {
  set_flag(cpu, I, false);
  return 0;
}

uint8_t CLV(CPU6502 *cpu) {
  set_flag(cpu, V, false);
  return 0;
}

uint8_t CMP(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  uint16_t temp = (uint16_t)cpu->A - (uint16_t)cpu->bus->current_value;
  set_flag(cpu, C, cpu->A >= cpu->bus->current_value);
  set_flag(cpu, Z, (temp & 0x00ff) == 0x00000);
  set_flag(cpu, N, temp & 0x0080);
  return 0;
}

uint8_t CPX(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  uint16_t temp = (uint16_t)cpu->X - (uint16_t)cpu->bus->current_value;
  set_flag(cpu, C, cpu->X >= cpu->bus->current_value);
  set_flag(cpu, Z, (temp & 0x00ff) == 0x0000);
  set_flag(cpu, N, temp & 0x0080);
  return 0;
}

uint8_t CPY(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  uint16_t temp = (uint16_t)cpu->Y - (uint16_t)cpu->bus->current_value;
  set_flag(cpu, C, cpu->Y >= cpu->bus->current_value);
  set_flag(cpu, Z, (temp & 0x00ff) == 0x0000);
  set_flag(cpu, N, temp & 0x0080);
  return 0;
}

uint8_t DEC(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  cpu->bus->current_value--;
  set_flag(cpu, Z, cpu->bus->current_value == 0);
  set_flag(cpu, N, cpu->bus->current_value & 0x80);
  return 0;
}

uint8_t DEX(CPU6502 *cpu) {
  cpu->X--;
  set_flag(cpu, Z, cpu->X == 0);
  set_flag(cpu, N, cpu->X & 0x80);
  return 0;
}

uint8_t DEY(CPU6502 *cpu) {
  cpu->Y--;
  set_flag(cpu, Z, cpu->Y == 0);
  set_flag(cpu, N, cpu->Y & 0x80);
  return 0;
}

uint8_t EOR(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  cpu->A ^= cpu->bus->current_value;
  set_flag(cpu, Z, cpu->A == 0);
  set_flag(cpu, N, cpu->A & 0x80);
  return 0;
}

uint8_t INC(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  uint16_t temp = cpu->bus->current_value + 1;
  set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
  set_flag(cpu, N, temp & 0x0080);
  return 0;
}

uint8_t INX(CPU6502 *cpu) {
  cpu->X++;
  set_flag(cpu, Z, cpu->X == 0x00);
  set_flag(cpu, N, cpu->X & 0x80);

  return 0;
};

uint8_t INY(CPU6502 *cpu) {
  cpu->Y++;
  set_flag(cpu, Z, cpu->Y == 0x00);
  set_flag(cpu, N, cpu->Y & 0x80);

  return 0;
};

uint8_t JMP(CPU6502 *cpu) {
  uint8_t addr_abs;
  cpu->PC = addr_abs;

  return 0;
}

uint8_t JSR(CPU6502 *cpu) { return 0; }

uint8_t LDA(CPU6502 *cpu) {
  cpu->A = 0xf;
  set_flag(cpu, Z, cpu->A == 0x00);
  return 0;
};

uint8_t LDX(CPU6502 *cpu) {
  cpu->X = cpu->A;
  return 0;
}

uint8_t LDY(CPU6502 *cpu) {
  cpu->Y = cpu->A;
  return 0;
}

uint8_t LSR(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  set_flag(cpu, C, cpu->bus->current_value & 0x01);
  cpu->bus->current_value >>= 1;
  set_flag(cpu, Z, cpu->bus->current_value == 0);
  set_flag(cpu, N, cpu->bus->current_value & 0x80);
  return 0;
}

uint8_t NOP(CPU6502 *cpu) { return 0; }

uint8_t ORA(CPU6502 *cpu) {
  hold_current_value(cpu->bus, cpu->PC);
  cpu->A |= cpu->bus->current_value;
  set_flag(cpu, Z, cpu->A == 0);
  set_flag(cpu, N, cpu->A & 0x80);
  return 0;
}

uint8_t PHA(CPU6502 *cpu) { return 0; }
uint8_t PHP(CPU6502 *cpu) { return 0; }
uint8_t PLA(CPU6502 *cpu) { return 0; }
uint8_t PLP(CPU6502 *cpu) { return 0; }
uint8_t ROL(CPU6502 *cpu) { return 0; }
uint8_t ROR(CPU6502 *cpu) { return 0; }
uint8_t RTI(CPU6502 *cpu) { return 0; }
uint8_t RTS(CPU6502 *cpu) { return 0; }
uint8_t SBC(CPU6502 *cpu) { return 0; }
uint8_t SEC(CPU6502 *cpu) { return 0; }
uint8_t SED(CPU6502 *cpu) { return 0; }
uint8_t SEI(CPU6502 *cpu) { return 0; }
uint8_t STA(CPU6502 *cpu) { return 0; }
uint8_t STX(CPU6502 *cpu) { return 0; }
uint8_t STY(CPU6502 *cpu) { return 0; }
uint8_t TAX(CPU6502 *cpu) { return 0; }
uint8_t TAY(CPU6502 *cpu) { return 0; }
uint8_t TSX(CPU6502 *cpu) { return 0; }
uint8_t TXA(CPU6502 *cpu) { return 0; }
uint8_t TXS(CPU6502 *cpu) { return 0; }

uint8_t TYA(CPU6502 *cpu) {
  cpu->A = cpu->Y;
  return 0;
}

uint8_t XXX(CPU6502 *cpu) { return 0; }

void clock(CPU6502 *cpu, Bus *bus) {
  if (cpu->cycles == 0) {
    int opcode = get_opcode(bus, cpu->PC);
    Instructions instruction = LOOKUP[opcode];
    cpu->cycles = instruction.cycles;
    uint8_t addrmode_additional_cycles = instruction.addrmode(cpu);
    uint8_t opmode_additional_cycles = instruction.opcode(cpu);
    cpu->cycles += addrmode_additional_cycles & opmode_additional_cycles;
  }

  cpu->cycles--;
};

void reset(CPU6502 *cpu) {
  cpu->PC = 0xff;
  cpu->A = 0;
  cpu->X = 0;
  cpu->Y = 0;
  cpu->SP = 0xFD;
};

void irq(CPU6502 *cpu, CPUStatusFlags flag) {
  if (get_flag(cpu, flag) == 0) {
    // write(0x0100 + cpu->SP--, (cpu->PC >> 8) & 0x00FF);
    cpu->SP--;

    // write(0x0100 + cpu->SP--, cpu->PC & 0x00FF);
    cpu->SP--;

    set_flag(cpu, B, 0);
    set_flag(cpu, U, 1);
    set_flag(cpu, I, 1);
    // write(0x0100 + cpu->SP, cpu->SR);
  }
};

// non-maskable interrupt
void nmi(CPU6502 *cpu) {
  set_flag(cpu, B, 0);
  set_flag(cpu, U, 1);
  set_flag(cpu, I, 1);

  uint16_t addr_abs = 0xFFFA;
  uint16_t lo = 0xFFAA;
  uint16_t hi = 0xFFAA;

  cpu->PC = (hi << 8) | lo;
  cpu->cycles = 8;
};

uint8_t IMP(CPU6502 *cpu) { return 0; }

// uses the next byte in the program counter as a value;
uint8_t IMM(CPU6502 *cpu) {
  cpu->PC++;
  return 0;
} // Immediate
//
uint8_t ZP0(CPU6502 *cpu) {
  uint16_t addr_abs = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;
  addr_abs &= 0x00FF;
  return 0;
} // Zero Page
//
uint8_t ZPX(CPU6502 *cpu) {
  uint16_t addr_abs = read_from_bus(cpu->bus, cpu->PC) + cpu->X;
  cpu->PC++;
  addr_abs = 0x00FF;
  return 0;
} // Zero Page,X

uint8_t ZPY(CPU6502 *cpu) {
  uint16_t addr_abs = read_from_bus(cpu->bus, cpu->PC) + cpu->Y;
  cpu->PC++;
  addr_abs = 0x00FF;
  return 0;
} // Zero Page,Y
//
uint8_t REL(CPU6502 *cpu) {
  uint16_t addr_rel = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  if (addr_rel & 0x80) {
    addr_rel |= 0xFF00;
  }

  return 0;
} // Relative
//
//
uint8_t ABS(CPU6502 *cpu) {
  uint16_t lo = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t hi = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t addr_abs = (hi << 8) | lo;

  return 0;
} // Absolute
//
uint8_t ABX(CPU6502 *cpu) {
  uint16_t lo = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t hi = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t addr_abs = (hi << 8) | lo;
  addr_abs += cpu->X;

  return 0;
} // Absolute,X
//
uint8_t ABY(CPU6502 *cpu) {
  uint16_t lo = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t hi = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t addr_abs = (hi << 8) | lo;
  addr_abs += cpu->Y;

  return 0;
} // Absolute,Y
uint8_t IND(CPU6502 *cpu) {
  uint16_t lo = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t hi = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t addr_abs = (hi << 8) | lo;
  return 0;
} // Indirect
//
uint8_t IZX(CPU6502 *cpu) {
  uint16_t addr = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t lo = read_from_bus(cpu->bus, (addr + (uint16_t)cpu->X) & 0x00FF);
  uint16_t hi = read_from_bus(cpu->bus, (addr + (uint16_t)cpu->X + 1) & 0x00FF);

  uint16_t addr_abs = (hi << 8) | lo;

  return 0;
} // (Indirect,X)
//
//
uint8_t IZY(CPU6502 *cpu) {
  uint16_t addr = read_from_bus(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t lo = read_from_bus(cpu->bus, (addr + (uint16_t)cpu->X) & 0x00FF);
  uint16_t hi = read_from_bus(cpu->bus, (addr + (uint16_t)cpu->X + 1) & 0x00FF);

  uint16_t addr_abs = (hi << 8) | lo;

  return 0;
} // (Indirect,X)

uint16_t read_ads_address(CPU6502 *cpu, uint16_t offset) {
  uint16_t lo = (uint16_t)read_from_bus(cpu->bus, offset);
  uint16_t hi = (uint16_t)read_from_bus(cpu->bus, offset + 1);

  return (hi << 8) | lo;
}

void onUpdate(CPU6502 *cpu) { clock(cpu, cpu->bus); };
