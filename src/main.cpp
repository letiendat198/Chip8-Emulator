#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "display.h"
#include <windows.h>
#include <iostream>

Chip8 chip8;

unsigned char boot[] = {
    0x12, 0x34, 0xF0, 0x88, 0x88, 0x88, 0x88, 0x70, 
    0x88, 0x88, 0x88, 0x70, 0xF0, 0x88, 0x88, 0xF0, 
    0x88, 0x70, 0x88, 0x88, 0x88, 0x70, 0x70, 0xA8, 
    0xA8, 0xA8, 0xA8, 0x60, 0x80, 0xE0, 0x80, 0x80, 
    0x70, 0x88, 0x88, 0x88, 0x70, 0x88, 0x88, 0x88, 
    0x88, 0x70, 0xF0, 0x88, 0x88, 0x88, 0x88, 0xF0, 
    0x88, 0x88, 0x88, 0xF0, 0x63, 0x05, 0x64, 0x06, 
    0x65, 0x08, 0x61, 0x02, 0x62, 0x0C, 0xA2, 0x02, 
    0xD1, 0x25, 0x81, 0x44, 0xF3, 0x1E, 0xD1, 0x25, 
    0x81, 0x54, 0xF3, 0x1E, 0xD1, 0x25, 0x81, 0x44, 
    0xF3, 0x1E, 0xD1, 0x25, 0x81, 0x44, 0xF3, 0x1E, 
    0xD1, 0x25, 0x81, 0x54, 0xF3, 0x1E, 0xD1, 0x25, 
    0x71, 0x04, 0xF3, 0x1E, 0xD1, 0x25, 0x81, 0x44, 
    0xF3, 0x1E, 0xD1, 0x25, 0x81, 0x44, 0xF3, 0x1E, 
    0xD1, 0x25, 0x81, 0x44, 0xF3, 0x1E, 0xD1, 0x25, 
    0x12, 0x78
};

int main(int argv, char** args){
    chip8.initialize();
    for(int i=0;i<sizeof(boot)/sizeof(char);i++){
        chip8.memory[i+512] = boot[i];
    }
    int rom_size = sizeof(boot);

    FILE *f = fopen("rom.ch8", "rb");
    fseek(f, 0L, SEEK_END);
    int size = ftell(f);
    if(size>0){
        fseek(f, 0L, SEEK_SET);
        unsigned char *buf = (unsigned char*)malloc(size);
        fread(buf, size,1, f);
        fclose(f);

        for(int i=0;i<size;i++){
            chip8.memory[i+512] = buf[i];
        }
        free(buf);
        rom_size = size;
    }
    

    printf("%d", chip8.pc);
    chip8.display.init();
    while(chip8.pc < rom_size+512){
        // printf("%d\n", chip8.pc);
        
        //Capture the keyboard at frame to check for key down -> ExA1 and Ex9E
        std::fill(chip8.key, chip8.key + sizeof(chip8.key), 0);
        chip8.display.get_input(chip8.key);
        if(chip8.emulate_cycle()) break;
    }
    
    return 0;
}