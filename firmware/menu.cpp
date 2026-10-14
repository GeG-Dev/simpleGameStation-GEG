#include "menu.h"
#include "chip8.h"
#include "settingsData.h"


uint16_t fileNum = 0;
uint16_t pointer = 0;
uint16_t currentPage = 0;
uint32_t animTmr = 0;
bool isSettings = false;
#define animTime 600
char pageData[MAX_ROWS][STR_MAX_LEN] = {};


bool hasEnd(const char *fileName , const char *end){
    uint8_t FileLen = strlen(fileName);
    uint8_t endLen = strlen(end);

    if (FileLen < endLen) return false;
    return strcasecmp(fileName + (FileLen - endLen) , end) == 0;
}

uint16_t getPageNum(uint16_t FNum){
    if(((FNum / MAX_ROWS)* MAX_ROWS) < FNum) return (FNum / MAX_ROWS);
    else return (FNum / MAX_ROWS) - 1;
}

bool sdData(uint16_t showListPage){
    File root = SD.open("/");
    if(!root) return false;
    File file = root.openNextFile();

    uint16_t skipCount = showListPage * MAX_ROWS;
    uint16_t skipped = 0;

    while(file && skipped < skipCount){
        if(!file.isDirectory() && hasEnd(file.name() , ".ch8")){
            skipped++;
        }
        file.close();
        file = root.openNextFile();
    }

    if(!file && skipped == 0 && skipCount == 0){
        root.close();
        return false;
    }

    tft.fillScreen(ST7735_BLACK);
    for(uint16_t i = 0; i< MAX_ROWS;i++){
        if(file){
            if(file.isDirectory() || !hasEnd(file.name() , ".ch8")){
                i--;
                file = root.openNextFile();
            }else{
                strncpy(pageData[i],file.name() , STR_MAX_LEN - 1);
                pageData[i][STR_MAX_LEN - 1] = '\0';
                tft.setCursor(8,i * 8);
                tft.print((showListPage * MAX_ROWS) + i + 1);
                tft.setCursor(24,i * 8);
                if(strlen(pageData[i]) > 16){
                    for(uint8_t c = 0; c < 13;c++){
                        tft.print(pageData[i][c]);
                    }
                    tft.print("...");
                }else{
                    tft.print(pageData[i]);

                }
                tft.setCursor(128,i * 8);
                tft.print(file.size());
                tft.setCursor(152,i * 8);
                tft.print("B");
                file = root.openNextFile();
            }
        }
        else{
            file.close();
            root.close();
            return true;
        }
    }
    file.close();
    root.close();
    return true;
}


uint16_t sdFilesNum(){
    fileNum = 0;
    File root = SD.open("/");
    if(!root) return 0;
    File file = root.openNextFile();
    while(file){
        if(!file.isDirectory() && hasEnd(file.name() , ".ch8")){
            fileNum++;
        }
        file.close();
        file = root.openNextFile();
    }
    root.close();
    return fileNum;
}


void initScreenFont(){
    tft.setTextSize(1);
    tft.setTextColor(ST7735_WHITE,ST7735_BLACK);
}
// chipSettings chSettings;
void drawSettingsMenu(){
    uint8_t pix = 0;
    uint8_t del = ((animTime * 10) / displayW);
    for(uint8_t i =0 ; i< 160;i++){
            delayMicroseconds(del * 100);
            tft.drawLine(displayW - pix-1,0,displayW - pix -1,displayH,ST7735_WHITE);
            tft.drawLine(displayW - pix,0,displayW - pix,displayH,ST7735_BLACK);
            pix ++;
        }
    tft.drawLine(displayW - pix,0,displayW - pix,displayH,ST7735_BLACK);

    drawData();
    // tft.print(dataSize);
}
void drawPointer(uint8_t x , uint8_t y ,  uint8_t isWhite){
    tft.setCursor(x,y);
    tft.setTextColor(isWhite ? ST7735_WHITE : ST7735_BLACK);
    tft.print(POINTER_SIGN);
    tft.setTextColor(ST7735_WHITE);
}
void menuControl(){
    if(!isSettings){
        if(chip8.keyState[0x8] == 1){
            drawPointer(0,pointer*8,0); 
            pointer++;
            if(pointer >= MAX_ROWS) {
                pointer = 0;
                currentPage ++;
                sdData(currentPage);
            }
            if((currentPage * MAX_ROWS) + pointer >= fileNum) pointer = (fileNum - 1) % MAX_ROWS;
            drawPointer(0,pointer*8,1); 
        }
        if(chip8.keyState[0xF] == 1){
                pointer = 1;
                drawSettingsMenu();
                isSettings = true;
                drawPointer(0,pointer*8,1); 
            }
        if(chip8.keyState[0x2] == 1){
            if(pointer > 0){
                drawPointer(0,pointer*8,0); 
                pointer--;
                drawPointer(0,pointer*8,1); 
            }
            else if(pointer == 0){
                if(currentPage > 0) {
                    pointer = 15;
                    currentPage --;
                    sdData(currentPage);
                    drawPointer(0,pointer*8,1); 
                }
            }
        }

    }else{
        if(chip8.keyState[0xF] == 1){
                tft.fillScreen(ST7735_BLACK);
                sdData(currentPage);
                isSettings = false;
                applyChanges();
                saveChanges();
                drawPointer(0,pointer*8,1); 
        }        
        if(chip8.keyState[0x8] == 1){
            if(pointer < dataSize){
                drawPointer(0,pointer*8,0); 
                pointer++;
                drawPointer(0,pointer*8,1); 
            }
        }
        if(chip8.keyState[0x2] == 1){
            if(pointer > 1){
                drawPointer(0,pointer*8,0); 
                pointer--;
                drawPointer(0,pointer*8,1); 
        }
        
        }
        if(chip8.keyState[0x6] == 1){
            changeData(pointer , 1);
            drawData();
            applyChanges();
            
        }
        if(chip8.keyState[0x4] == 1){
            changeData(pointer , -1);
            drawData();
            applyChanges();
        }   
    }
}