
#include "SD.h"
#ifndef SD_CARD_H
#define SD_CARD_H

#define SD_CS_PIN 16

extern File myFile;
bool initFile(const char *path);
void initSd();
void resetChip();


#endif