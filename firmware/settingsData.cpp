#include "chip8.h"
#include "settingsData.h"
#include <Adafruit_ST7735.h>
extern Adafruit_ST7735 tft;
Preferences pref;

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

gameStation myStation = {100,1};

sample myData[] = {
    {"brightness",&myStation.brightness,TYPE_BYTE,10,250},
    {"showFPS",&myStation.showFPS,TYPE_BOOL,0,1}
};

const int dataSize = sizeof(myData) / sizeof(myData[0]);
bool isShowFPS = 1;

void saveChanges(){
    pref.begin("settings" , false);
    pref.putUChar("bright" , myStation.brightness);
    pref.putBool("FPS" , myStation.showFPS);
    pref.end();
}
void loadSettings(){
    pref.begin("settings" , true);
    myStation.brightness = pref.getUChar("bright" , 100);
    myStation.showFPS = pref.getBool("FPS" , true);
    pref.end();
    applyChanges();
}

void drawData(){
    tft.setCursor(8,0);
    tft.print("SETTINGS:");
    tft.setCursor(0,16);
    // tft.print(dataSize);
    for(uint8_t i = 0; i < dataSize;i++){
        tft.setCursor(8 , (i + 1) * 8);
        tft.print(myData[i].label);
        tft.print(": ");

        switch (myData[i].size){
        case TYPE_BOOL:
            tft.print(*(bool*)myData[i].ptr ? "YES" : "NO");
            break;
            case TYPE_BYTE:
            tft.print(*(byte*)myData[i].ptr);
            break;
            case TYPE_INT:
            tft.print(*(int*)myData[i].ptr);
            break;
        
        default:
            break;
        }
        
    }
}

void changeData(uint16_t dataPtr , int8_t interval){
    tft.fillRect(8,dataPtr*8,150,8,ST7735_BLACK);
    switch (myData[(dataPtr-1)].size){
        case TYPE_BOOL:
            *(bool*)myData[(dataPtr-1)].ptr += interval;
            break;
        case TYPE_BYTE:
            *(byte*)myData[(dataPtr-1)].ptr += (interval * 10);
            break;
        case TYPE_INT:
            *(int*)myData[(dataPtr-1)].ptr += interval;
            break;
        }
    drawData();
}

void applyChanges(){
    analogWrite(27,*(byte*)myData[0].ptr);
    isShowFPS = *(bool*)myData[1].ptr; 
}