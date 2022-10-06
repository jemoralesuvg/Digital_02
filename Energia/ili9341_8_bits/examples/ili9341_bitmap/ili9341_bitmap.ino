
#include <stdint.h>
#include <ili9341_8_bits.h>
// Pines usados
//#define LCD_RST PD_0
//#define LCD_CS PD_1
//#define LCD_RS PD_2
//#define LCD_WR PD_3
//#define LCD_RD PA_6
// hay que modificar los pines en la librería
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};

#include "bitmaps.h"

void setup() {
  
  lcdSetDataPins(DPINS);
  Serial.begin(9600);
  Serial.println("Iniciando pantalla...");
  lcdInit();
  Serial.println("Pintando negro");
  lcdClear(BLACK);
  
  for(int x = 0; x <319; x++){
    
    LCD_Bitmap(x, 207, 16, 16, tile2);
    LCD_Bitmap(x, 223, 16, 16, tile);
    x += 15;
 }
    LCD_Print("Proyecto 2", 50, 100, 2, PINK, BLACK);
    LCD_Print("Video juego", 100, 150, 1, PINK, BLACK);
}

void loop() {
  for(int x = 0; x <320-32; x++){
    delay(5);
    int anim2 = (x/25)%2;

    uint16_t anim_speed = x/5;
    uint16_t anim_frame = anim_speed%8;
    
    LCD_Sprite(x, 175, 16, 32, mario,8, anim_frame,1, 0);
    V_line( x -1, 175, 31, BLACK);
 
  }
  for(int x = 320-32; x >0; x--){
    delay(5);
    uint16_t anim_speed = x/5;
    uint16_t anim_frame = anim_speed%8;
    
    LCD_Sprite(x, 175, 16, 32, mario,8, anim_frame,0, 0);
    V_line( x +16, 175, 31, BLACK);
  } 
}
