#include "Arduino.h"

#include <Adafruit_ST7735.h>
#ifndef CHIP8_H
#define CHIP8_H

#define SCREEN_W 64
#define SCREEN_H 32

#define SCHIP_SCREEN_W 128
#define SCHIP_SCREEN_H 64

#define displayW 160
#define displayH 128

#define startScreenX  displayW / 2 - SCREEN_W 
#define startScreenY displayH / 2 - SCREEN_H 

extern Adafruit_ST7735 tft;
extern const uint8_t fontset[80];
extern const uint8_t schip_fontset[100];

struct Chip8
{
    uint8_t RAM[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t PC;
    uint8_t SP;
    uint16_t stack[16];
    
    uint8_t tmrReg;
    uint8_t soundReg;
    
    uint8_t screenBuf[SCHIP_SCREEN_W * SCHIP_SCREEN_H];
    uint8_t keyState[16];
    uint8_t RPL[16];
    void cycle();
    void initFont();
    void scrollRight();
    void scrollLeft();
    void drawScreen();
    bool resolutionMode = 0; // 0 - chip 8 / 1 - schip
    bool isDraw = 0;
};
// enum paramSize {TYPE_INT , TYPE_BOOL , TYPE_BYTE};
// struct sample {
//     const char* label; //name
//     void* ptr; //var pointer
//     paramSize size; //name
//     uint8_t min; 
//     uint8_t max;
// };

// struct gameStation
// {
//     uint8_t brightness;
//     bool showFPS;
// };

// gameStation myStation = {255,1};

// sample myData[] = {
//     {"brightness",&myStation.brightness,TYPE_BYTE,10,250},
//     {"showFPS",&myStation.showFPS,TYPE_BOOL,0,1}
// };

// const int dataSize = sizeof(myData) / sizeof(myData[0]);

void drawBorder();

extern int frameCount;
extern Chip8 chip8;
#endif