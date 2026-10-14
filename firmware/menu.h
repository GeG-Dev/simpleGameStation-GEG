#include"sd_card.h"
#include <Adafruit_ST7735.h>
extern Adafruit_ST7735 tft;

#ifndef MENU_H
#define MENU_H

#define POINTER_SIGN ">"
#define MAX_ROWS 16
#define STR_MAX_LEN 48
#define FONT_SIZE 8
#define X_OFFSET 2

extern char pageData[MAX_ROWS][STR_MAX_LEN];

extern uint16_t fileNum;
extern uint16_t pointer;
extern uint16_t currentPage ;


bool hasEnd(const char *fileName , const char *end);
uint16_t sdFilesNum();
void printData();
void drawMenu();
void initScreenFont();
void drawSettingsMenu();
void drawPointer(uint8_t x , uint8_t y ,  uint8_t isWhite);
void menuControl();
uint16_t getPageNum(uint16_t FNum);
bool sdData(uint16_t showListPage);

#endif