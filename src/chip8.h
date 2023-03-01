#include "display.h"

#ifndef CHIP8 
#define CHIP8
class Chip8{
    public:
        unsigned short opcode;
        unsigned char memory[4096];
        unsigned char V[16];
        unsigned short I;
        unsigned short pc;
        unsigned char gfx[64*32];
        unsigned char delay_timer;
        unsigned char sound_timer;
        unsigned short stack[16];
        unsigned short sp;
        unsigned char key[16];
        int skip_counter;
        void initialize();
        int emulate_cycle();
        Display display;
};
#endif