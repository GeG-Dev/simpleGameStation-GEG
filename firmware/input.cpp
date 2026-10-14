#include "input.h"
#include "chip8.h"

void initBtn(){
    pinMode(upBtn , INPUT_PULLUP);
    pinMode(downBtn , INPUT_PULLUP);
    pinMode(rightBtn , INPUT_PULLUP);
    pinMode(leftBtn, INPUT_PULLUP);
    pinMode(aBtn , INPUT_PULLUP);
    pinMode(bBtn , INPUT_PULLUP);
    pinMode(cBtn , INPUT_PULLUP);
    pinMode(dBtn , INPUT_PULLUP);
    pinMode(pauseBtn , INPUT_PULLUP);
    pinMode(mainMenuBtn , INPUT_PULLUP);
}
void checkButton(){
    memset(chip8.keyState,0,sizeof(chip8.keyState));
    if(digitalRead(upBtn) == 0) chip8.keyState[0x2] = 1;
    if(digitalRead(downBtn) == 0) chip8.keyState[0x8] = 1;
    if(digitalRead(rightBtn) == 0) chip8.keyState[0x6] = 1;
    if(digitalRead(leftBtn) == 0) chip8.keyState[0x4] = 1;
    if(digitalRead(aBtn) == 0) chip8.keyState[0x5] = 1;
    if(digitalRead(bBtn) == 0) chip8.keyState[0x1] = 1;
    if(digitalRead(cBtn) == 0) chip8.keyState[0xA] = 1;
    if(digitalRead(dBtn) == 0) chip8.keyState[0x0] = 1;
    if(digitalRead(pauseBtn) == 0) chip8.keyState[0xF] = 1;
    if(digitalRead(mainMenuBtn) == 0) chip8.keyState[0xE] = 1;
    
}
void SchipCheckButton(){
    memset(chip8.keyState,0,sizeof(chip8.keyState));
    if(digitalRead(upBtn) == 0) chip8.keyState[0x1] = 1;
    if(digitalRead(downBtn) == 0) chip8.keyState[0x2] = 1;
    if(digitalRead(rightBtn) == 0) chip8.keyState[0xc] = 1;
    if(digitalRead(leftBtn) == 0) chip8.keyState[0x3] = 1;
    if(digitalRead(aBtn) == 0) chip8.keyState[0xa] = 1;
    if(digitalRead(bBtn) == 0) chip8.keyState[0x0] = 1;
    if(digitalRead(cBtn) == 0) chip8.keyState[0x0] = 1;
    if(digitalRead(dBtn) == 0) chip8.keyState[0x0] = 1;
    if(digitalRead(pauseBtn) == 0) chip8.keyState[0xF] = 1;
    if(digitalRead(mainMenuBtn) == 0) chip8.keyState[0xE] = 1;
    
}