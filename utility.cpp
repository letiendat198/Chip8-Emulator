#include "utility.h"
#include <iostream>
#include <stdlib.h>
#include <string.h>

int Utility::mattoa(int x, int y, int col){
    return y*col + x;
}
int* Utility::atomat(int n, int col){
    int *a = (int*)malloc(sizeof(int) * 2);
    a[0] = n%col;
    a[1] = n/col;
    return a;
}
short Utility::keymap(const char* key){
    if(strcmp("1", key)==0) return 0x01;
    else if(strcmp("2", key)==0) return 0x02;
    else if(strcmp("3", key)==0) return 0x03;
    else if(strcmp("4", key)==0) return 0x0c;
    else if(strcmp("Q", key)==0) return 0x04;
    else if(strcmp("W", key)==0) return 0x05;
    else if(strcmp("E", key)==0) return 0x06;
    else if(strcmp("R", key)==0) return 0x0d;
    else if(strcmp("A", key)==0) return 0x07;
    else if(strcmp("S", key)==0) return 0x08;
    else if(strcmp("D", key)==0) return 0x09;
    else if(strcmp("F", key)==0) return 0x0e;
    else if(strcmp("Z", key)==0) return 0x0a;
    else if(strcmp("X", key)==0) return 0x00;
    else if(strcmp("C", key)==0) return 0x0b;
    else if(strcmp("V", key)==0) return 0x0f;
    else return 0x00;
}