#include "Arduino.h"
#ifndef INPUT_H
#define INPUT_H

#define upBtn 26
#define downBtn 33
#define rightBtn 25
#define leftBtn 32
#define aBtn 21
#define bBtn 5
#define cBtn 17
#define dBtn 22
#define pauseBtn 18
#define mainMenuBtn 19


void initBtn();
void checkButton();
void SchipCheckButton();

#endif