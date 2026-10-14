#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_ST7735.h>
#include "chip8.h"
#include "sd_card.h"
#include"input.h"
#include "settingsData.h"
#include"menu.h"
#include "imges.h"

#define MOSI_PIN 13
#define MISO_PIN 12
#define CLK_PIN 14
#define CS_PIN 15
#define RST_PIN 2
#define AN_PIN 4

#define BUZ_PIN 23
#define BUZ_FREQ 500

#define CPU_DELAY 5

enum currentState { STATE_MENU , STATE_GAME};
currentState curState = STATE_MENU;

uint32_t tmr;
uint32_t cpuTmr;
uint32_t frameTmr;
unsigned long lastTime = 0;
float fps = 0;

File myFile;
Adafruit_ST7735 tft = Adafruit_ST7735(CS_PIN , AN_PIN , RST_PIN);


void setup() {
    Serial.begin(115200);
    SPI.begin(CLK_PIN,MISO_PIN,MOSI_PIN,CS_PIN);
    pinMode(27 , OUTPUT);
    analogWrite(27,100);       
    tft.initR(INITR_BLACKTAB);
    tft.fillScreen(ST7735_BLACK);
    tft.setRotation(3);
    
    tft.drawBitmap(0,0,BGESP,displayW,displayH , ST7735_WHITE , ST7735_BLACK);
    
    delay(600);    
    
    chip8.initFont();
    initBtn();
    
    initSd();
    Serial.println(sdFilesNum());
    randomSeed(analogRead(A0));
    
    initScreenFont();
    loadSettings();   
    sdData(0);
    drawPointer(0,pointer * 8,1);

    

}


void gameLoop(){
    frameTmr = micros();
    if(micros() - cpuTmr >= CPU_DELAY){
        chip8.cycle();
        drawBorder();
        if(chip8.resolutionMode == 1){
            SchipCheckButton();
        }else{
            checkButton();
            delay(1);
        }
        cpuTmr = micros();
    }
    if(millis() - tmr >= 16){
        if(chip8.tmrReg > 0)chip8.tmrReg--;
        if(chip8.soundReg > 0){
            chip8.soundReg--;
            tone(BUZ_PIN , BUZ_FREQ);
        }else{
            noTone(BUZ_PIN);
        }
        tmr = millis();
        if(chip8.isDraw)chip8.drawScreen();
    }
    unsigned long currentTime = millis();
    if (currentTime - lastTime >= 1000 && isShowFPS) {
        fps = frameCount / ((currentTime - lastTime) / 1000.0);
        frameCount = 0;
        lastTime = currentTime;
        tft.fillRect(0,0,80,8,ST7735_BLACK);
        tft.setCursor(0,1);
        tft.print(fps);
        // Serial.printf("FPS: %.2f\n", fps);
    }
}

void loop() {
    switch (curState){
        case STATE_MENU:
            while(1) {
                menuControl();
                checkButton();
                delay(100);
                if(chip8.keyState[0x5] == 1){
                    initFile(pageData[pointer]);
                    break;
                }
            }   
            tft.fillScreen(ST7735_BLACK);
            curState = STATE_GAME;
            break;
            case STATE_GAME:
                gameLoop();
                if(chip8.keyState[0xE] == 1){
                    curState = STATE_MENU;
                    pointer = 0;
                    sdData(0);
                    drawPointer(0,pointer,1);
                    break;
                }
            break;
        }

    // delay(1);
}

