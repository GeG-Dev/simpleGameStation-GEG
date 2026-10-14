#include "sd_card.h"
#include "chip8.h"
void resetChip(){
    chip8.PC = 0x200;
    chip8.SP = 0;
    chip8.I = 0;
    chip8.resolutionMode = 0;
    memset(&chip8.screenBuf[0], 0 , SCHIP_SCREEN_H * SCHIP_SCREEN_W);
    memset(&chip8.V[0], 0 , sizeof(chip8.V));
    memset(&chip8.RPL[0], 0 , sizeof(chip8.RPL));
    memset(&chip8.stack[0], 0 , sizeof(chip8.stack));
}
bool initFile(const char *path){
    if(!SD.begin(SD_CS_PIN))Serial.println("err");
    else Serial.println("good");
    char fullPath[256];
    snprintf(fullPath, sizeof(fullPath), "/%s", path);
    resetChip();
    
    myFile = SD.open(fullPath , FILE_READ);
    if(myFile){
        uint16_t  readBytes = 0;
        while(myFile.available() && 0x200 + readBytes < 4096){
            chip8.RAM[0x200 + readBytes] = myFile.read();
            readBytes++;
        }
        myFile.close();
        return true ;
    }
    return false; 

}

void initSd(){
    if(!SD.begin(SD_CS_PIN)){
        tft.setCursor(6,1);
        tft.print("Loading...");
        while(!SD.begin(SD_CS_PIN)){delay(1000);}
    }
}