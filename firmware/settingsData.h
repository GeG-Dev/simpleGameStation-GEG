
#include "Arduino.h"
#include "Preferences.h"
#ifndef SETTINGSDATA_H
#define SETTINGSDATA_H

enum paramSize {TYPE_INT , TYPE_BOOL , TYPE_BYTE};
struct sample {
    const char* label; //name
    void* ptr; //var pointer
    paramSize size; //name
    uint8_t min; 
    uint8_t max;
};

struct gameStation
{
    uint8_t brightness;
    bool showFPS;
};

extern gameStation myStation;
extern sample myData[];
extern const int dataSize ;
extern bool isShowFPS;

void drawData();
void changeData(uint16_t dataPtr , int8_t interval);
void applyChanges();
void loadSettings();
void saveChanges();

#endif