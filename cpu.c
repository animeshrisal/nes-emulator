#include "./cpu.h"
#include "bus.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

void create_cpu(CPU6502 *cpu, Bus *bus) {
  cpu->A = 0;
  cpu->X = 0;
  cpu->Y = 0;
  cpu->PC = 0x0000;
  cpu->SP = 0xfd;

  cpu->cycles = 7;
  cpu->bus = bus;
}

void reset_cpu(CPU6502 *cpu) {

  uint16_t addr_abs = 0x0000;
  uint16_t lo = read_from_memory(cpu->bus, addr_abs + 0);
  uint16_t hi = read_from_memory(cpu->bus, addr_abs + 1);

  cpu->PC = (hi << 8) | lo;
}

uint8_t get_opcode(Bus *bus, uint16_t addr) {
  return read_from_memory(bus, addr);
}

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

uint8_t ADC(CPU6502 *cpu, uint16_t addr) {
  uint8_t current_value = read_from_memory(cpu->bus, addr);
  uint16_t temp =
      (uint16_t)cpu->A + (uint16_t)current_value + (uint16_t)get_flag(cpu, C);

  set_flag(cpu, C, temp > 255);
  set_flag(cpu, V,
           (~((uint16_t)cpu->A ^ (uint16_t)current_value) &
            ((uint16_t)cpu->A ^ (uint16_t)temp)) &
               0x0080);

  set_flag(cpu, N, temp & 0x80);

  cpu->A = temp & 0x00FF;
  return 1;
}

uint8_t AND(CPU6502 *cpu, uint16_t addr) {
  uint8_t value = read_from_memory(cpu->bus, addr);
  cpu->A &= value;

  set_flag(cpu, Z, cpu->A == 0x00);
  set_flag(cpu, N, cpu->A & 0x80);

  return 0;
}

uint8_t ASL(CPU6502 *cpu, uint16_t addr) {
  uint8_t value = read_from_memory(cpu->bus, addr);

  uint16_t temp = (strcmp(cpu->current_addressing_mode, "IMP") ? cpu->A : value)
                  << 1;

  set_flag(cpu, C, (temp & 0xFF00) > 0);
  set_flag(cpu, Z, (temp & 0x00FF) == 0x80);
  set_flag(cpu, N, temp & 0x80);

  if (strcmp(cpu->current_addressing_mode, "IMP")) {
    cpu->A = temp;
  } else {
    write_to_memory(cpu->bus, addr, temp);
  }

  return 0;
}

uint8_t BCC(CPU6502 *cpu, uint16_t addr) {
  if (get_flag(cpu, C) == 0) {
    cpu->cycles++;
    uint16_t addr_abs = cpu->PC + addr;

    if ((addr_abs & 0xFF00) != (cpu->PC & 0xFF00)) {
      cpu->cycles++;
    }
    cpu->PC = addr_abs;
  }
  return 0;
}

uint8_t BCS(CPU6502 *cpu, uint16_t addr) {
  if (get_flag(cpu, C) == 1) {
    cpu->cycles++;
    uint16_t addr_abs = cpu->PC + addr;

    if ((addr_abs & 0xFF00) != (cpu->PC & 0xFF00)) {
      cpu->cycles++;
    }
    cpu->PC = addr_abs;
  }
  return 0;
}

uint8_t BEQ(CPU6502 *cpu, uint16_t addr) {

  if (get_flag(cpu, Z) == 1) {
    cpu->cycles++;
    uint16_t addr_abs = cpu->PC + addr;

    if ((addr_abs & 0xFF00) != (cpu->PC & 0xFF00)) {
      cpu->cycles++;
    }
    cpu->PC = addr_abs;
  }
  return 0;
}

uint8_t BIT(CPU6502 *cpu, uint16_t addr) {}

uint8_t BMI(CPU6502 *cpu, uint16_t addr) {

  if (get_flag(cpu, N) == 1) {
    cpu->cycles++;

    cpu->PC = addr;
  }

  return 0;
}

uint8_t BNE(CPU6502 *cpu, uint16_t addr) {
  if (get_flag(cpu, Z) == 0) {
    cpu->cycles++;

    cpu->PC = addr;
  }

  return 0;
}
uint8_t BPL(CPU6502 *cpu, uint16_t addr) {
  if (get_flag(cpu, N) == 0) {
    cpu->cycles++;
    cpu->PC = addr;
  }
  return 0;
}

uint8_t BRK(CPU6502 *cpu, uint16_t addr) {
  cpu->PC++;

  set_flag(cpu, I, 1);
  write_to_memory(cpu->bus, 0x0100 + cpu->SP, (cpu->PC >> 8) & 0x00FF);
  cpu->SP--;
  write_to_memory(cpu->bus, 0x0100 + cpu->SP, cpu->PC & 0x00FF);
  cpu->SP--;

  set_flag(cpu, B, 1);
  write_to_memory(cpu->bus, 0x0100 + cpu->SP, cpu->SR);
  cpu->SP--;
  set_flag(cpu, B, 0);

  cpu->PC = (uint16_t)read_from_memory(cpu->bus, 0xFFFE) |
            ((uint16_t)read_from_memory(cpu->bus, 0xFFFF) << 8);
  return 0;
}

uint8_t BVC(CPU6502 *cpu, uint16_t addr) {
  if (get_flag(cpu, V) == 0) {
    cpu->cycles++;

    cpu->PC = addr;
  }
  return 0;
}

uint8_t BVS(CPU6502 *cpu, uint16_t addr) {
  if (get_flag(cpu, V) == 0) {
    cpu->cycles++;

    cpu->PC = addr;
  }
  return 0;
}

// clear carry flag
uint8_t CLC(CPU6502 *cpu, uint16_t addr) {
  set_flag(cpu, C, 0);
  return 0;
};

uint8_t CLD(CPU6502 *cpu, uint16_t addr) {
  set_flag(cpu, D, 0);
  return 0;
}

uint8_t CLI(CPU6502 *cpu, uint16_t addr) {
  set_flag(cpu, I, 0);
  return 0;
}

uint8_t CLV(CPU6502 *cpu, uint16_t addr) {
  set_flag(cpu, V, 0);
  return 0;
}

uint8_t CMP(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  uint16_t temp = (uint16_t)cpu->A - (uint16_t)cpu->bus->current_value;
  set_flag(cpu, C, cpu->A >= cpu->bus->current_value);
  set_flag(cpu, Z, (temp & 0x00ff) == 0x00000);
  set_flag(cpu, N, temp & 0x0080);
  return 0;
}

uint8_t CPX(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  uint16_t temp = (uint16_t)cpu->X - (uint16_t)cpu->bus->current_value;
  set_flag(cpu, C, cpu->X >= cpu->bus->current_value);
  set_flag(cpu, Z, (temp & 0x00ff) == 0x0000);
  set_flag(cpu, N, temp & 0x0080);
  return 0;
}

uint8_t CPY(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  uint16_t temp = (uint16_t)cpu->Y - (uint16_t)cpu->bus->current_value;
  set_flag(cpu, C, cpu->Y >= cpu->bus->current_value);
  set_flag(cpu, Z, (temp & 0x00ff) == 0x0000);
  set_flag(cpu, N, temp & 0x0080);
  return 0;
}

uint8_t DEC(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  cpu->bus->current_value--;
  set_flag(cpu, Z, cpu->bus->current_value == 0);
  set_flag(cpu, N, cpu->bus->current_value & 0x80);
  return 0;
}

uint8_t DEX(CPU6502 *cpu, uint16_t addr) {
  cpu->X--;
  set_flag(cpu, Z, cpu->X == 0);
  set_flag(cpu, N, cpu->X & 0x80);
  return 0;
}

uint8_t DEY(CPU6502 *cpu, uint16_t addr) {
  cpu->Y--;
  set_flag(cpu, Z, cpu->Y == 0);
  set_flag(cpu, N, cpu->Y & 0x80);
  return 0;
}

uint8_t EOR(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, cpu->PC);
  cpu->A ^= cpu->bus->current_value;
  set_flag(cpu, Z, cpu->A == 0);
  set_flag(cpu, N, cpu->A & 0x80);
  return 0;
}

uint8_t INC(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  uint16_t temp = cpu->bus->current_value + 1;
  set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
  set_flag(cpu, N, temp & 0x0080);
  return 0;
}

uint8_t INX(CPU6502 *cpu, uint16_t addr) {
  cpu->X++;
  set_flag(cpu, Z, cpu->X == 0x00);
  set_flag(cpu, N, cpu->X & 0x80);

  return 0;
};

uint8_t INY(CPU6502 *cpu, uint16_t addr) {
  cpu->Y++;
  set_flag(cpu, Z, cpu->Y == 0x00);
  set_flag(cpu, N, cpu->Y & 0x80);

  return 0;
};

uint8_t JMP(CPU6502 *cpu, uint16_t addr) {
  cpu->PC = addr;

  return 0;
}

uint8_t JSR(CPU6502 *cpu, uint16_t addr) { return 0; }

uint8_t LDA(CPU6502 *cpu, uint16_t addr) {
  cpu->A = 0xf;
  set_flag(cpu, Z, cpu->A == 0x00);
  return 0;
};

uint8_t LDX(CPU6502 *cpu, uint16_t addr) {
  cpu->X = cpu->A;
  return 0;
}

uint8_t LDY(CPU6502 *cpu, uint16_t addr) {
  cpu->Y = cpu->A;
  return 0;
}

uint8_t LSR(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  uint8_t value = cpu->bus->current_value;
  set_flag(cpu, C, value & 0x0001);
  uint8_t temp = value >> 1;
  set_flag(cpu, Z, (temp & 0x00FF) == 0x0000);
  set_flag(cpu, N, (temp & 0x0080));
  return 0;
}

uint8_t NOP(CPU6502 *cpu, uint16_t addr) { return 0; }

uint8_t ORA(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  cpu->A |= cpu->bus->current_value;
  set_flag(cpu, Z, cpu->A == 0);
  set_flag(cpu, N, cpu->A & 0x80);
  return 0;
}

uint8_t PHA(CPU6502 *cpu, uint16_t addr) {
  write_to_memory(cpu->bus, 0x0100 + cpu->SP, cpu->A);

  return 0;
}

uint8_t PHP(CPU6502 *cpu, uint16_t addr) {
  write_to_memory(cpu->bus, 0x0100 + cpu->SP,
                  cpu->SR | 0x30); // Push status register with B flag set
  cpu->SP--;
  return 0;
}

uint8_t PLA(CPU6502 *cpu, uint16_t addr) {
  cpu->SP++;
  cpu->A = read_from_memory(cpu->bus, 0x0100 + cpu->SP);
  set_flag(cpu, Z, cpu->A == 0);
  set_flag(cpu, N, cpu->A & 0x80);
  return 0;
}

uint8_t PLP(CPU6502 *cpu, uint16_t addr) {
  cpu->SP++;
  cpu->SR = read_from_memory(cpu->bus, 0x0100 + cpu->SP) & 0xEF |
            0x20; // Restore status register with B flag clear
  return 0;
}

uint8_t ROL(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  uint16_t temp = (cpu->bus->current_value << 1) | get_flag(cpu, C);
  set_flag(cpu, C, temp & 0xFF00);
  cpu->bus->current_value = temp & 0x00FF;
  set_flag(cpu, Z, cpu->bus->current_value == 0);
  set_flag(cpu, N, cpu->bus->current_value & 0x80);
  return 0;
}

uint8_t ROR(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  uint16_t temp = (get_flag(cpu, C) << 7) | (cpu->bus->current_value >> 1);
  set_flag(cpu, C, cpu->bus->current_value & 0x01);
  cpu->bus->current_value = temp & 0x00FF;
  set_flag(cpu, Z, cpu->bus->current_value == 0);
  set_flag(cpu, N, cpu->bus->current_value & 0x80);
  return 0;
}

uint8_t RTI(CPU6502 *cpu, uint16_t addr) {
  cpu->SP++;
  cpu->SR = read_from_memory(cpu->bus, 0x0100 + cpu->SP);

  cpu->SR &= ~B;
  cpu->SR &= ~U;

  cpu->SP++;

  cpu->PC = (uint16_t)read_from_memory(cpu->bus, 0x0100 + cpu->SP);
  cpu->SP++;

  cpu->PC |= (uint16_t)read_from_memory(cpu->bus, 0x0100 + cpu->SP) << 8;

  return 0;
}

uint8_t RTS(CPU6502 *cpu, uint16_t addr) {
  cpu->SP++;
  cpu->PC = read_from_memory(cpu->bus, 0x0100 + cpu->SP);
  cpu->SP++;
  cpu->PC |= (read_from_memory(cpu->bus, 0x0100 + cpu->SP) << 8);
  cpu->PC++;
  return 0;
}

uint8_t SBC(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, addr);
  uint16_t value = cpu->bus->current_value ^ 0x00FF;
  uint16_t temp = (uint16_t)cpu->A + value + (uint16_t)get_flag(cpu, C);
  set_flag(cpu, C, temp & 0xFF00);
  set_flag(cpu, Z, (temp & 0x00FF) == 0);
  set_flag(cpu, V, (temp ^ (uint16_t)cpu->A) & (temp ^ value) & 0x0080);
  set_flag(cpu, N, temp & 0x0080);
  cpu->A = temp & 0x00FF;
  return 0;
}

uint8_t SEC(CPU6502 *cpu, uint16_t addr) {
  set_flag(cpu, C, 1);
  return 0;
}

uint8_t SED(CPU6502 *cpu, uint16_t addr) {
  set_flag(cpu, D, 1);
  return 0;
}

uint8_t SEI(CPU6502 *cpu, uint16_t addr) {
  set_flag(cpu, I, 1);
  return 0;
}

uint8_t STA(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, cpu->PC);
  cpu->bus->current_value = cpu->A;
  return 0;
}

uint8_t STX(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, cpu->PC);
  cpu->bus->current_value = cpu->X;
  return 0;
}

uint8_t STY(CPU6502 *cpu, uint16_t addr) {
  hold_current_value(cpu->bus, cpu->PC);
  cpu->bus->current_value = cpu->Y;
  return 0;
}

uint8_t TAX(CPU6502 *cpu, uint16_t addr) {
  cpu->X = cpu->A;
  set_flag(cpu, Z, cpu->X == 0);
  set_flag(cpu, N, cpu->X & 0x80);
  return 0;
}

uint8_t TAY(CPU6502 *cpu, uint16_t addr) {
  cpu->Y = cpu->A;
  set_flag(cpu, Z, cpu->Y == 0);
  set_flag(cpu, N, cpu->Y & 0x80);
  return 0;
}

uint8_t TSX(CPU6502 *cpu, uint16_t addr) {
  cpu->X = cpu->SP;
  set_flag(cpu, Z, cpu->X == 0);
  set_flag(cpu, N, cpu->X & 0x80);
  return 0;
}

uint8_t TXA(CPU6502 *cpu, uint16_t addr) {
  cpu->A = cpu->X;
  set_flag(cpu, Z, cpu->A == 0);
  set_flag(cpu, N, cpu->A & 0x80);
  return 0;
}

uint8_t TXS(CPU6502 *cpu, uint16_t addr) {
  cpu->SP = cpu->X;
  return 0;
}

uint8_t TYA(CPU6502 *cpu, uint16_t addr) {
  cpu->A = cpu->Y;
  set_flag(cpu, Z, cpu->A == 0);
  set_flag(cpu, N, cpu->A & 0x80);
  return 0;
}

uint8_t XXX(CPU6502 *cpu, uint16_t addr) { return 0; }

void clock(CPU6502 *cpu, Bus *bus) {
  if (cpu->cycles == 0) {

    int opcode = get_opcode(bus, cpu->PC);
    set_flag(cpu, U, 1);
    cpu->PC++;

    Instructions instruction = LOOKUP[opcode];
    printf("%s\n", instruction.name);
    printf("%x\n", cpu->PC);
    cpu->cycles = instruction.cycles;
    strcpy(cpu->current_addressing_mode, instruction.name);

    uint16_t get_address = instruction.addrmode(cpu);
    uint8_t opmode_additional_cycles = instruction.opcode(cpu, get_address);

    // cpu->cycles += opmode_additional_cycles;
  }

  cpu->cycles--;
};

void reset(CPU6502 *cpu) {
  cpu->PC = 0xfffc;
  cpu->A = 0;
  cpu->X = 0;
  cpu->Y = 0;
  cpu->SP = 0xFD;
};

void irq(CPU6502 *cpu, CPUStatusFlags flag) {
  if (get_flag(cpu, flag) == 0) {
    write_to_memory(cpu->bus, 0x0100 + cpu->SP--, (cpu->PC >> 8) & 0x00FF);
    cpu->SP--;

    write_to_memory(cpu->bus, 0x0100 + cpu->SP--, cpu->PC & 0x00FF);
    cpu->SP--;

    set_flag(cpu, B, 0);
    set_flag(cpu, U, 1);
    set_flag(cpu, I, 1);
    write_to_memory(cpu->bus, 0x0100 + cpu->SP, cpu->SR);

    cpu->SP--;

    uint16_t addr_abs = 0xFFFE;
    uint16_t lo = read_from_memory(cpu->bus, addr_abs);
    uint16_t hi = read_from_memory(cpu->bus, addr_abs + 1);

    cpu->PC = (hi << 8) | lo;
    cpu->cycles = 7;
  }
};

// non-maskable interrupt
void nmi(CPU6502 *cpu) {
  write_to_memory(cpu->bus, 0x0100 + cpu->SP--, (cpu->PC >> 8) & 0x00FF);
  cpu->SP--;

  write_to_memory(cpu->bus, 0x0100 + cpu->SP--, cpu->PC & 0x00FF);
  cpu->SP--;

  set_flag(cpu, B, 0);
  set_flag(cpu, U, 1);
  set_flag(cpu, I, 1);

  write_to_memory(cpu->bus, 0x0100 + cpu->SP, cpu->SR);
  cpu->SP--;

  uint16_t addr_abs = 0xFFFA;
  uint16_t lo = read_from_memory(cpu->bus, addr_abs);
  uint16_t hi = read_from_memory(cpu->bus, addr_abs + 1);

  cpu->PC = (hi << 8) | lo;

  cpu->cycles = 8;
};

uint16_t IMP(CPU6502 *cpu) { return 0; }

// uses the next byte in the program counter as a value;
uint16_t IMM(CPU6502 *cpu) {
  cpu->PC++;
  return cpu->PC;
} // Immediate
//
uint16_t ZP0(CPU6502 *cpu) {
  uint16_t addr_abs = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;
  addr_abs &= 0x00FF;
  return addr_abs;
} // Zero Page
//
uint16_t ZPX(CPU6502 *cpu) {
  uint16_t addr_abs = read_from_memory(cpu->bus, cpu->PC) + cpu->X;
  cpu->PC++;
  addr_abs &= 0x00FF;
  return addr_abs;
} // Zero Page,X

uint16_t ZPY(CPU6502 *cpu) {
  uint16_t addr_abs = read_from_memory(cpu->bus, cpu->PC) + cpu->Y;
  cpu->PC++;
  addr_abs &= 0x00FF;
  return addr_abs;
} // Zero Page,Y
//
uint16_t REL(CPU6502 *cpu) {
  uint16_t addr_rel = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  if (addr_rel & 0x80) {
    addr_rel |= 0xFF00;
  }

  return addr_rel;
} // Relative
//
//
uint16_t ABS(CPU6502 *cpu) {
  uint16_t lo = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t hi = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  return (hi << 8) | lo;
} // Absolute
//
uint16_t ABX(CPU6502 *cpu) {
  uint16_t lo = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t hi = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t addr_abs = (hi << 8) | lo;
  addr_abs += cpu->X;

  return addr_abs;
} // Absolute,X
//
//
uint16_t ABY(CPU6502 *cpu) {
  uint16_t lo = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t hi = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t addr_abs = (hi << 8) | lo;
  addr_abs += cpu->Y;

  return addr_abs;
} // Absolute,Y

uint16_t IND(CPU6502 *cpu) {
  uint16_t lo = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t hi = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t addr_abs = (hi << 8) | lo;
  return addr_abs;
} // Indirect

uint16_t IZX(CPU6502 *cpu) {
  uint16_t addr = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t lo = read_from_memory(cpu->bus, (addr + (uint16_t)cpu->X) & 0x00FF);
  uint16_t hi =
      read_from_memory(cpu->bus, (addr + (uint16_t)cpu->X + 1) & 0x00FF);

  uint16_t addr_abs = (hi << 8) | lo;

  return addr_abs;
} // (Indirect,X)
//
//
uint16_t IZY(CPU6502 *cpu) {
  uint16_t addr = read_from_memory(cpu->bus, cpu->PC);
  cpu->PC++;

  uint16_t lo = read_from_memory(cpu->bus, (addr + (uint16_t)cpu->X) & 0x00FF);
  uint16_t hi =
      read_from_memory(cpu->bus, (addr + (uint16_t)cpu->X + 1) & 0x00FF);

  uint16_t addr_abs = (hi << 8) | lo;

  return addr_abs;
} // (Indirect,Y)
//

uint16_t read_ads_address(CPU6502 *cpu, uint16_t offset) {
  uint16_t lo = (uint16_t)read_from_memory(cpu->bus, offset);
  uint16_t hi = (uint16_t)read_from_memory(cpu->bus, offset + 1);

  return (hi << 8) | lo;
}

void onUpdate(CPU6502 *cpu) { clock(cpu, cpu->bus); };

void prepare_code(char (*code)[100], CPU6502 *cpu) {
  char str[15];
  uint8_t lo = 0x00, hi = 0x00;
  uint8_t value = 0x00;
  uint16_t addr = 0x8000;
  while (addr >= 0x8000 && addr <= 0xffff) {
    int opcode = get_opcode(cpu->bus, addr);
    int current_address = addr;
    Instructions instr = LOOKUP[opcode];
    addr++;
    if (instr.addrmode == &IMP) {
      sprintf(code[current_address], "%04X: %s", addr, instr.name);
    } else if (instr.addrmode == &IMM) {
      value = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s #$%04x", addr, instr.name,
              value);
      addr++;
    } else if (instr.addrmode == &ZP0) {
      lo = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s $%02X", addr, instr.name, lo);

      addr++;
    } else if (instr.addrmode == &ZPX) {
      lo = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s $%02X, X", addr, instr.name, lo);

      addr++;
    } else if (instr.addrmode == &ZPY) {
      lo = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s $%02X, Y", addr, instr.name, lo);
      addr++;

    } else if (instr.addrmode == &REL) {
      value = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s $%02X [$%04X]", addr, instr.name,
              value, addr + value);
      addr++;

    } else if (instr.addrmode == &ABS) {
      lo = read_from_memory(cpu->bus, addr);
      addr++;
      hi = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s $%04X", addr, instr.name,
              (hi << 8) | lo);
      addr++;

    } else if (instr.addrmode == &ABX) {
      lo = read_from_memory(cpu->bus, addr);
      addr++;
      hi = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s $%04X, X", addr, instr.name,
              (hi << 8) | lo);
      addr++;

    } else if (instr.addrmode == &ABY) {
      lo = read_from_memory(cpu->bus, addr);
      addr++;
      hi = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s $%04X, Y", addr, instr.name,
              (hi << 8) | lo);
      addr++;

    } else if (instr.addrmode == &IND) {
      lo = read_from_memory(cpu->bus, addr);
      addr++;
      hi = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s ($%04X)", addr, instr.name,
              (hi << 8) | lo);

      addr++;
    } else if (instr.addrmode == &IZX) {
      lo = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s ($%02X, X)", addr, instr.name,
              lo);

      addr++;
    } else if (instr.addrmode == &IZY) {
      lo = read_from_memory(cpu->bus, addr);
      sprintf(code[current_address], "%04X: %s ($%02X), Y", addr, instr.name,
              lo);

      addr++;
    }

    printf("%s \n", code[current_address]);
  }
}
