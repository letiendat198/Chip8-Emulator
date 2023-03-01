#include "SDL2/SDL.h"
#include "stdio.h"
#include "display.h"


void Display::init(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(640,320, 0, &window, &renderer);
}
void Display::draw(){
    SDL_SetRenderDrawColor(renderer, 255,255,255,0);
    // SDL_RenderDrawLine(renderer,0,0,200,200);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_RenderFillRect(renderer, &rect);
}
void Display::erase(){
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    // SDL_RenderDrawLine(renderer,0,0,200,200);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_RenderFillRect(renderer, &rect);
}
void Display::show(){
    SDL_RenderPresent(renderer);
}
void Display::finish(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    quit = true;
}
void Display::wait(){
    while(SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                finish();
                break;
        }
    }
}
void Display::clear(){
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderClear(renderer);
}
const char* Display::wait_input(){
    while(1){
        while(SDL_WaitEvent(&event)){
            if(event.type == SDL_KEYUP){
                return SDL_GetKeyName(event.key.keysym.sym);
            }
            if(event.type == SDL_QUIT){
                finish();
                return NULL;
            }
        }      
    }
    return NULL;
}
void Display::get_input(unsigned char key_state[]){
    while(SDL_PollEvent(&event)){
        if(event.type == SDL_QUIT){
                finish();
                return;
        }
    }
    const Uint8* key = SDL_GetKeyboardState(NULL);
    if(key[SDL_SCANCODE_1]) key_state[0x01]=1;
    if(key[SDL_SCANCODE_2]) key_state[0x02]=1;
    if(key[SDL_SCANCODE_3]) key_state[0x03]=13;
    if(key[SDL_SCANCODE_4]) key_state[0x0c]=1;
    if(key[SDL_SCANCODE_Q]) key_state[0x04]=1;
    if(key[SDL_SCANCODE_W]) key_state[0x05]=1;
    if(key[SDL_SCANCODE_E]) key_state[0x06]=1;
    if(key[SDL_SCANCODE_R]) key_state[0x0d]=1;
    if(key[SDL_SCANCODE_A]) key_state[0x07]=1;
    if(key[SDL_SCANCODE_S]) key_state[0x08]=1;
    if(key[SDL_SCANCODE_D]) key_state[0x09]=1;
    if(key[SDL_SCANCODE_F]) key_state[0x0e]=1;
    if(key[SDL_SCANCODE_Z]) key_state[0x0a]=1;
    if(key[SDL_SCANCODE_X]) key_state[0x00]=1;
    if(key[SDL_SCANCODE_C]) key_state[0x0b]=1;
    if(key[SDL_SCANCODE_V]) key_state[0x0f]=1;
}