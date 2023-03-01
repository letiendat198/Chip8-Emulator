#include <iostream>
#include <algorithm>
#include <windows.h>
#include <time.h>
#include "chip8.h"
#include "utility.h"

// Chip8{
//     public:
//         unsigned short opcode;
//         unsigned char memory[4096];
//         unsigned char V[16];
//         unsigned short I;
//         unsigned short pc;
//         unsigned char gfx[64*32];
//         unsigned char delay_timer;
//         unsigned char sound_timer;
//         unsigned short stack[16];
//         unsigned short sp;
//         unsigned char key[16];
//         void initialize();
//         void emulate_cycle();
// };
Utility utility;
void Chip8::initialize(){
    pc = 0x200;
    I = 0;
    sp = 0;
    opcode = 0;
    std::fill(memory, memory + sizeof(memory), 0);
    std::fill(stack, stack + sizeof(stack), 0);
    std::fill(gfx, gfx + sizeof(gfx),0);
    std::fill(key, key + sizeof(key), 0);
    unsigned char chip8_fontset[80] =
        { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };
    for(int i=80;i<160;i++){
        memory[i] = chip8_fontset[i-80];
    }
    skip_counter=0;
}

int Chip8::emulate_cycle(){
    if(display.quit == true) return 1;

    // if(skip_counter!=0){
    //     skip_counter--;
    //     return 0;
    // }
    // skip_counter=5000;

    opcode = memory[pc] << 8 | memory[pc+1];
    printf("%04x ", opcode);

    bool draw_flag = false;

    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x00FF){
                case 0x00E0: //CLS
                    std::fill(gfx, gfx + sizeof(gfx), 0);
                    // display.clear();
                    printf("CLS");
                    pc+=2;
                    break;
                case 0x00EE: //RET
                    pc = stack[--sp];
                    pc+=2;
                    printf("RET");
                    break;
            }
            break;
        case 0xA000: //LD I, addr
            printf("LD I, addr");
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        case 0xB000: //JP + V0
            printf("JP V0, addr");
            //stack[sp++] = pc;
            pc = (opcode & 0x0FFF) + V[0];
            break;
        case 0xC000://RAN
            { 
            printf("RAN");
            srand(time(NULL));
            char ran = rand() % (255 - 0 + 1);
            V[(opcode & 0x0F00) >> 8] = ran & (opcode & 0x00FF);
            pc+=2;
            break;
            }
        case 0x1000: //JP addr
            printf("JP addr");
            pc = opcode & 0x0FFF;
            break;
        case 0x2000: //CALL
            printf("CALL");
            stack[sp++] = pc;
            pc = opcode & 0x0FFF;
            break;
        case 0x3000: //SE Vx,byte
            printf("SE Vx,byte");
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)){
                pc+=4;
            }
            else pc += 2;
            break;
        case 0x4000: //SNE Vx,byte
            printf("SNE Vx,byte");
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)){
                pc+=4;
            }
            else pc+=2;
            break;
        case 0x5000: //SE Vx,Vy
            printf("SE Vx,Vy");
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
                pc+=4;
            }
            else pc+=2;
            break; 
        case 0x6000: //LD Vx,byte
            printf("LD Vx,byte");
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc+=2;
            break;
        case 0x7000: //ADD Vx,byte
            printf("ADD Vx,byte");
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc+=2;
            break;
        case 0x8000:
            switch (opcode & 0x000F){
                case 0x0000: //LD Vx,Vy
                    printf("LD Vx,Vy");
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc+=2;
                    break;
                case 0x0001: //OR Vx,Vy
                    printf("OR Vx,Vy");
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                    V[15] = 0;
                    pc+=2;
                    break;
                case 0x0002: //AND Vx,Vy
                    printf("AND Vx,Vy");
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                    V[15] = 0;
                    pc+=2;
                    break;
                case 0x0003: //XOR Vx,Vy
                    printf("XOR Vx,Vy");
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
                    V[15] = 0;
                    pc+=2;
                    break;
                case 0x0004: //ADD Vx,Vy
                    printf("ADD Vx,Vy");
                    
                    if((V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4])>0xFF){
                        V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) & 0xFF;
                        V[15] = 1;
                    } 
                    else{
                        V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4]) & 0xFF;
                        V[15] = 0;
                    } 
                    pc+=2;
                    break;
                case 0x0005: //SUB Vx,Vy
                    printf("SUB Vx,Vy");
                    if(V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4]){
                        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
                        V[15] = 1;
                    }
                    else{
                        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
                        V[15] = 0;
                    }
                    pc+=2;
                    break;
                case 0x0006://SHR Vx {,Vy}
                {
                    printf("SHR Vx {,Vy}");
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    char cache = V[(opcode & 0x0F00) >> 8] & 0b1;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] / 2;
                    if(cache) V[15] = 1;
                    else V[15] = 0; 
                    pc += 2;
                    break;
                }
                case 0x0007: //SUBN Vx, Vy
                    printf("SUBN Vx, Vy");
                    if(V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4]){
                        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                        V[15] = 1;
                    }
                    else{
                        V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                        V[15] = 0;
                    } 
                    pc+=2;
                    break;
                case 0x000E: //SHL Vx {, Vy}
                {
                    printf("SHL Vx {, Vy}");
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    char cache = V[(opcode & 0x0F00) >> 8] & 0b10000000;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] * 2;
                    if(cache) V[15] = 1;
                    else V[15] = 0;
                    pc += 2;
                    break;
                }
                default:
                    printf("NOT YET IMPLEMENTED");
                    pc+=2;
                    break;
            } 
            break;
        case 0x9000: //SNE Vx, Vy
            printf("SNE Vx, Vy");
            if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) pc+=4;
            else pc+=2;
            break;
        case 0xD000://DRW Vx,Vy
          {    
            short x = V[(opcode & 0x0F00) >> 8] % 64; //Wrap around when x is out of screen
            short y = V[(opcode & 0x00F0) >> 4] % 32; //Wrap around when y is out of screen
            short height = opcode & 0x000F;
            V[0xF] = 0;
            for(int row = 0; row<height;row++){
                if((y+row)>31) break; //If out of bound don't render cause clipping
                short pixel = memory[I+ row];
                for(int col=0;col<8;col++){
                    if((x+col) > 63) break; //If out of bound don't render cause clipping
                    if((pixel & (0x80 >> col))!=0 && (x+col + ((y+row)*64)) < (64*32)){ //Fail safe when array overflow -> Prevent segfault
                        if(gfx[x+col + ((y+row)*64)]==1) V[0xF] = 1;
                        gfx[x+col + ((y+row)*64)] ^=1;
                    }
                }
            }
            draw_flag = true;
            pc+=2;
            break;
          }
        case 0xE000:
            switch (opcode & 0x00FF){
                case 0x009E: //Check key press if yes skip next
                    if(key[V[(opcode & 0x0F00) >> 8]] != 0) pc+=4;
                    else pc+=2;
                    break;
                case 0x00A1:
                    if(key[V[(opcode & 0x0F00) >> 8]] == 0) pc+=4;
                    else pc+=2;
                    break;
            }
        case 0xF000:
            switch (opcode & 0x00FF){
                case 0x0007: //LD Vx, DT
                    printf("LD Vx, DT");
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc+=2;
                    break;
                case 0x000A: //LD Vx, K 
                {
                    printf("LD Vx, K");
                    const char *key_input = display.wait_input();
                    printf(" %s", key_input);
                    if(key_input!=NULL){
                        short keycode = utility.keymap(key_input);
                        V[(opcode & 0x0F00) >> 8] = keycode;
                        printf(" %02x", keycode);
                    }
                    pc+=2;
                    break;
                }
                case 0x0015:
                    printf("LD DT, Vx");
                    delay_timer = V[(opcode & 0x0F00) >> 8];
                    pc+=2;
                    break;
                case 0x0018:
                    printf("LD ST, Vx");
                    sound_timer = V[(opcode & 0x0F00) >> 8];
                    pc+=2;
                    break;
                case 0x001E:
                    printf("ADD I, Vx");
                    I = I + V[(opcode & 0x0F00) >> 8];
                    pc+=2;
                    break;
                case 0x0029:
                    printf("LD F, Vx"); //Point I to starting memory address of sprite Vx
                    I = 80 + (V[(opcode & 0x0F00) >> 8] * 5);
                    pc+=2;
                    break;
                case 0x0033:
                    printf("LD B, Vx");
                    memory[I+2] = V[(opcode & 0x0F00) >> 8] % 10;
                    memory[I+1] = (V[(opcode & 0x0F00) >> 8] % 100) / 10;
                    memory[I] = (V[(opcode & 0x0F00) >> 8] % 1000) / 100;
                    pc+=2;
                    break;
                case 0x0055:
                    printf("LD [I], Vx");
                    for(int i=0;i<=(opcode & 0x0F00)>>8;i++){
                        memory[I] = V[i];
                        I++;
                    }
                    pc+=2;
                    break;
                case 0x0065:
                    printf("LD Vx, [I]");
                    for(int i=0;i<=(opcode & 0x0F00)>>8;i++){
                        V[i] = memory[I]; 
                        I++;
                    }
                    pc+=2;
                    break;
            }
            break;
        default:
            printf("NOT YET IMPLEMENTED");
            pc+=2;
            break;
    }
    if(delay_timer > 0) delay_timer--;
    if(sound_timer >0) {
        sound_timer--;
        printf(" BEEP!");
    }
    if(draw_flag){
        // printf("\n");
        
        for(int i =0; i<64*32; i++){
            // if(gfx[i]) printf("#");
            // else printf(" ");
            // if((i+1)%64==0) printf("\n");
            int *cord = utility.atomat(i, 64);  //Can be removed for better performance
            
            display.x = *(cord)*10;
            display.y = *(cord+1)*10;
            // printf(" %d %d ", display.x, display.y);
            display.width = 10;
            display.height = 10;
            
            if(gfx[i]==1){
                display.draw();
            }
            else{
                display.erase();
            }
            free(cord);
        }
        display.show();
        display.wait();
    }
    printf("\n");
    Sleep(1.5);
    return 0;
}