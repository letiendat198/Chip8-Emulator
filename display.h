#include "SDL2/SDL.h"

#ifndef DISPLAY 
#define DISPLAY
class Display{
    public:
        int x;
        int y;
        int width;
        int height;
        bool quit;
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Event event;
        void init();
        void draw();
        void erase();
        void finish();
        void wait();
        void clear();
        void show();
        const char* wait_input();
        void get_input(unsigned char key[]);
};
#endif