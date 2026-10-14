
#include "chip8.h"
int frameCount = 0;
Chip8 chip8;
const uint8_t fontset[80] = {
0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
0x20, 0x60, 0x20, 0x20, 0x70, // 1
0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
0x90, 0x90, 0xF0, 0x10, 0x10, // 4
0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
0xF0, 0x10, 0x20, 0x40, 0x40, // 7
0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
0xF0, 0x90, 0xF0, 0x90, 0x90, // A
0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
0xF0, 0x80, 0x80, 0x80, 0xF0, // C
0xE0, 0x90, 0x90, 0x90, 0xE0, // D
0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};
const uint8_t schip_fontset[100] = {
    0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C, // 0
    0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, // 1
    0x3C, 0x7E, 0xC3, 0x03, 0x0F, 0x1E, 0x38, 0x60, 0xFF, 0xFF, // 2
    0x3C, 0x7E, 0xC3, 0x03, 0x1E, 0x1E, 0x03, 0xC3, 0x7E, 0x3C, // 3
    0x06, 0x0E, 0x1E, 0x36, 0x66, 0xC6, 0xFF, 0xFF, 0x06, 0x06, // 4
    0xFF, 0xFF, 0xC0, 0xC0, 0xFC, 0xFE, 0x03, 0xC3, 0x7E, 0x3C, // 5
    0x3E, 0x7C, 0xC0, 0xC0, 0xFC, 0xFE, 0xC3, 0xC3, 0x7E, 0x3C, // 6
    0xFF, 0xFF, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x60, 0x60, // 7
    0x3C, 0x7E, 0xC3, 0xC3, 0x7E, 0x7E, 0xC3, 0xC3, 0x7E, 0x3C, // 8
    0x3C, 0x7E, 0xC3, 0xC3, 0x7F, 0x3F, 0x03, 0x03, 0x3E, 0x7C  // 9
};

void Chip8::initFont(){
    memcpy(&RAM[0x0000],fontset,sizeof(fontset));
    // memcpy(&RAM[0x0050],schip_fontset,sizeof(schip_fontset));
}

void Chip8::cycle(){
    uint16_t opcode = (RAM[PC] << 8) | RAM[PC + 1];
    PC += 2;

    uint16_t addr = opcode & 0x0FFF;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;
    uint8_t kk = opcode & 0x00FF;

    switch (opcode & 0xF000){
    case 0x0000:
        switch (opcode){
            case 0x00E0: //clr display
                memset(screenBuf , 0 , sizeof(screenBuf));
                tft.fillScreen(ST7735_BLACK);
                break;
            case 0x00EE:
                SP--;
                PC = stack[SP];
                break;
            case 0x00FF:
                resolutionMode = 1;
                break;    
            case 0x00FB:
                scrollRight();
                break;
            case 0x00FC:
                scrollLeft();
                break;
            case 0x00FE:
                resolutionMode = 0;
                break;    
            case 0x00FD:
                ESP.restart();
                break;
            case 0x00C0:
            {
                uint8_t N = (opcode & 0x000F);
                memmove(&chip8.screenBuf[N * SCHIP_SCREEN_W] , &chip8.screenBuf[0],(SCHIP_SCREEN_H - N )* SCHIP_SCREEN_W);
                memset(&screenBuf[0], 0 , N * SCHIP_SCREEN_W);
                drawScreen();
                break;
            }
        }
        break;
    case 0x1000:
        PC = addr;
        break;
    case 0x2000:
        stack[SP] = PC;
        SP++;
        PC = addr;
        break;
    case 0x3000:
        if(V[x] == kk) PC += 2;
        break;
    case 0x4000:
        if(V[x] != kk) PC += 2;
        break;
    case 0x5000:
        if(V[x] == V[y]) PC += 2;
        break;
    case 0x6000:
        V[x] = kk;
        break;
    case 0x7000:
        V[x] = V[x] + kk;
        break;
    case 0x8000:
        switch (opcode & 0x000F){
            case 0x0000:
                V[x] = V[y];
                break;
            case 0x0001:
                V[x] = V[x] | V[y];
                break;
            case 0x0002:
                V[x] = V[x] & V[y];
                break;
            case 0x0003:
                V[x] = V[x] ^ V[y];
                break;
            case 0x0004:{
                uint16_t sum = V[x] + V[y];
                V[0xF] = (sum > 255) ? 1 : 0;
                V[x] = sum & 0x00FF;
                break;
            }
            case 0x0005:
                V[0xF] = ( V[x] >= V[y]) ? 1 : 0;
                V[x] = V[x] - V[y];
                break;
            case 0x0006:
                V[0xF] = V[x] & 0x01;
                V[x] >>= 1;
                break;
            case 0x0007:
                V[0xF] = (V[y] >= V[x]) ? 1 : 0;
                V[x] = V[y] - V[x];
                break;
            case 0x000E:
                V[0xF] = (V[x] & 0x80) >> 7;
                V[x] <<= 1;
                break;
        }
        break;
    case 0x9000:
        if (V[x] != V[y]) PC += 2;
        break;
    case 0xA000:
        I = addr;
        break;
    case 0xB000:
        PC = addr + V[0];
        break;
    case 0xC000:
        V[x] = (random(0,255) & kk);
        break;
    case 0xD000:{
            uint8_t n = opcode & 0x000F;
            uint8_t curW = resolutionMode ? SCHIP_SCREEN_W : SCREEN_W;
            uint8_t curH = resolutionMode ? SCHIP_SCREEN_H : SCREEN_H;
            uint8_t scale = resolutionMode ? 1 : 2;

            uint8_t xPos = V[x] % curW;
            uint8_t yPos = V[y] % curH;

            uint8_t rows = (n == 0) ? 16 : n;
            uint8_t cols = (n == 0) ? 16 : 8;

            V[0xF] = 0;

            for(uint8_t row = 0; row < rows;row ++){
                uint16_t data;
                if(n == 0){
                    data = (RAM[I + row * 2] << 8 )| RAM[ I + row * 2 + 1];
                }else{
                    data = RAM[row + I] << 8;
                }
                
                // uint8_t data = RAM[row + I];
                for(uint8_t col = 0; col < cols;col++){
                    if((data & (0x8000 >> col)) != 0){
                        uint8_t xPoint = (xPos + col) % curW;
                        uint8_t yPoint = (yPos + row) % curH;

                        uint16_t bufPos = (yPoint * SCHIP_SCREEN_W) + xPoint;
                        if(screenBuf[bufPos] == 1) V[0xF] = 1;
                        
                            screenBuf[bufPos] ^= 1;
                        // tft.fillRect(xPoint * scale + startScreenX,yPoint * scale + startScreenY,scale,scale,screenBuf[bufPos] ? ST7735_WHITE : ST7735_BLACK);
                    }
                }  
            }
            frameCount++;
            isDraw = 1;
        }
        break;
    case 0XE000:
        switch (opcode & 0x00FF){
        case 0x9E:
            if((keyState[V[x] & 0x0F]) != 0) PC += 2;
            break;
        case 0xA1:
            if((keyState[V[x] & 0x0F] ) == 0) PC += 2;
            break;
        
        }
        break;
    case 0xF000:
        switch (opcode & 0x00FF){
            case 0x07:
                V[x] = tmrReg;
                break;
            case 0x0A:{
                    uint8_t isTrue = false;
                    for(uint8_t pos = 0; pos <= 15;pos++){
                        if (keyState[pos] != 0) {
                            isTrue = true;
                            V[x] = pos;
                            break;
                        } 
                    }
                    if (!isTrue) PC-= 2;
                    
                }
                break;
            case 0x15:
                tmrReg = V[x];
                break;
            case 0x18:
                soundReg = V[x];
                break;
            case 0x1E:
                I = I + V[x];
                break;
            case 0x29:
                I = (V[x] & 0x0F) * 5;
                break;
            case 0x30:
                I = 0x0050 + ((V[x] & 0x0F) * 10);
                break;
            case 0x33:
                RAM[I] = V[x] / 100;
                RAM[I+ 1] = (V[x] % 100)/ 10;
                RAM[I + 2] = V[x] % 10;
                break;
            case 0x55:
                for(uint8_t pos = 0; pos <= x;pos++){
                    RAM[I + pos] = V[pos];
                }
                break;
            case 0x65:
                for(uint8_t pos = 0; pos <= x;pos++){
                    V[pos] = RAM[I + pos];
                }
                break;
            case 0x75:{
                uint8_t count = (x > 7) ? 7 : x;
                for(uint8_t i =0 ; i <= count;i++){
                    RPL[i] = V[i];
                } 
            }
                break;
            case 0x85:{
                uint8_t count = (x > 7) ? 7 : x;
                for(uint8_t i =0 ; i <= count;i++){
                    V[i] = RPL[i];
                } 
            }
                break;
        }
        break;
    }
}

void drawBorder(){
    tft.drawRect(startScreenX , startScreenY,SCREEN_W * 2,SCREEN_H * 2,ST7735_WHITE);
}
void Chip8::drawScreen(){
    uint8_t width = (resolutionMode == 0) ? SCREEN_W : SCHIP_SCREEN_W; 
    uint8_t height = (resolutionMode == 0) ? SCREEN_H : SCHIP_SCREEN_H; 
    uint8_t scale = resolutionMode  ? 1 : 2;

    
    static uint16_t frame[SCHIP_SCREEN_H * SCHIP_SCREEN_W * 2];
    for(uint8_t y = 0; y < height;y++){
        for(uint8_t x = 0; x < width;x++){
            uint16_t color = (screenBuf[y*SCHIP_SCREEN_W + x]) ? ST7735_WHITE : ST7735_BLACK;

            if(scale == 2){
                uint16_t fx = x*2;
                uint16_t fy = y*2;
                frame[fy * SCHIP_SCREEN_W + fx] = color;
                frame[fy * SCHIP_SCREEN_W + (fx + 1)] = color;
                frame[(fy + 1) * SCHIP_SCREEN_W + fx] = color;
                frame[(fy + 1) * SCHIP_SCREEN_W + (fx + 1)] = color;

            }else{
                frame[y * SCHIP_SCREEN_W + x] = color;
            }
        }
    }
    tft.drawRGBBitmap(startScreenX,startScreenY,frame,SCHIP_SCREEN_W,SCHIP_SCREEN_H);
    isDraw = 0;
}

void Chip8::scrollRight(){
    uint8_t width = (resolutionMode == 0) ? SCREEN_W : SCHIP_SCREEN_W; 
    uint8_t height = (resolutionMode == 0) ? SCREEN_H : SCHIP_SCREEN_H;
    
    for(uint8_t y = 0; y < height;y++){
        uint16_t row = (y * width);

        memmove(&chip8.screenBuf[row + 4] , &chip8.screenBuf[row] , width - 4);
        memset(&chip8.screenBuf[row], 0 , 4);
    }
    drawScreen();
}
void Chip8::scrollLeft(){
    uint8_t width = (resolutionMode == 0) ? SCREEN_W : SCHIP_SCREEN_W; 
    uint8_t height = (resolutionMode == 0) ? SCREEN_H : SCHIP_SCREEN_H; 
    
    for(uint8_t y = 0; y < height;y++){
        uint16_t row = (y * width);

        memmove(&chip8.screenBuf[row] , &chip8.screenBuf[row + 4] ,width - 4);
        memset(&chip8.screenBuf[row + width - 4], 0 , 4);
    }
    drawScreen();
}