/*
 * ili9341
 * Autor: Jos? Eduardo Morales
 * Creaci?n: 4/10/2022	Modificaci?n: 4/10/2022
 *	Basado en ili9341_light.c de "tabur"
 */
#include "ili9341_8_bits_fast.h"
#include <stdbool.h>
#include <stdint.h>
#include <energia.h>
#include <font.h>
#include <TM4C123GH6PM.h>

enum {
  MemoryAccessControlNormalOrder,
  MemoryAccessControlReverseOrder
} MemoryAccessControlRefreshOrder;

enum {
	MemoryAccessControlColorOrderRGB,
	MemoryAccessControlColorOrderBGR
} MemoryAccessControlColorOrder;

static unsigned char lcdPortraitConfig = 0;
static unsigned char lcdLandscapeConfig = 0;
static unsigned char lcdPortraitMirrorConfig = 0;
static unsigned char lcdLandscapeMirrorConfig = 0;

#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PA_6

const int DataPins[8] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};

void lcdSetDataPins(int DPINS[8])
{
	for (uint8_t i = 0; i< 8; i++)
	{
		DataPins[i] = DPINS[i];
	}
}
// Write an 8 bit command to the IC driver
static void lcdWriteCommand(unsigned char command)
{
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}

// Write an 8 bit data word to the IC driver
static void lcdWriteData(unsigned short data)
{
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}

static void lcdReset(void)
{
	lcdWriteCommand(ILI9341_SOFTRESET);
	delay(50);
}

static unsigned char lcdBuildMemoryAccessControlConfig(
                        bool rowAddressOrder,
                        bool columnAddressOrder,
                        bool rowColumnExchange,
                        bool verticalRefreshOrder,
                        bool colorOrder,
                        bool horizontalRefreshOrder)
{
  unsigned char value 				= 0;
  if(horizontalRefreshOrder) value 	|= ILI9341_MADCTL_MH;
  if(colorOrder) value 				|= ILI9341_MADCTL_BGR;
  if(verticalRefreshOrder) value 	|= ILI9341_MADCTL_ML;
  if(rowColumnExchange) value 		|= ILI9341_MADCTL_MV;
  if(columnAddressOrder) value 		|= ILI9341_MADCTL_MX;
  if(rowAddressOrder) value 		|= ILI9341_MADCTL_MY;
  return value;
}

void lcdInit(void)
{
	
	pinMode(LCD_CS, OUTPUT);
	pinMode(LCD_RS, OUTPUT);
	pinMode(LCD_WR, OUTPUT);
	pinMode(LCD_RD, OUTPUT);
	pinMode(LCD_RST, OUTPUT);
	for (uint8_t i = 0; i<8; i++)
	{
		pinMode(DataPins[i], OUTPUT);
	}
	digitalWrite(LCD_CS, HIGH);
	digitalWrite(LCD_RS, HIGH);
	digitalWrite(LCD_WR, HIGH);
	digitalWrite(LCD_RD, HIGH);
	digitalWrite(LCD_RST, HIGH);
	delay(5);
	digitalWrite(LCD_RST, LOW);
	delay(20);
	digitalWrite(LCD_RST, HIGH);
	delay(150);
	digitalWrite(LCD_CS, LOW);
  
  lcdPortraitConfig = lcdBuildMemoryAccessControlConfig(
                                                    MemoryAccessControlNormalOrder,		// rowAddressOrder
                                                    MemoryAccessControlReverseOrder,	// columnAddressOrder
                                                    MemoryAccessControlNormalOrder,		// rowColumnExchange
                                                    MemoryAccessControlNormalOrder,		// verticalRefreshOrder
                                                    MemoryAccessControlColorOrderBGR,	// colorOrder
                                                    MemoryAccessControlNormalOrder);	// horizontalRefreshOrder

  lcdLandscapeConfig = lcdBuildMemoryAccessControlConfig(
                                                    MemoryAccessControlNormalOrder,		// rowAddressOrder
                                                    MemoryAccessControlNormalOrder,		// columnAddressOrder
                                                    MemoryAccessControlReverseOrder,	// rowColumnExchange
                                                    MemoryAccessControlNormalOrder,		// verticalRefreshOrder
                                                    MemoryAccessControlColorOrderBGR,	// colorOrder
                                                    MemoryAccessControlNormalOrder);	// horizontalRefreshOrder

  lcdPortraitMirrorConfig = lcdBuildMemoryAccessControlConfig(
		  	  	  	  	  	  	  	  	  	  	  	MemoryAccessControlReverseOrder,	// rowAddressOrder
		                                            MemoryAccessControlNormalOrder,		// columnAddressOrder
		                                            MemoryAccessControlNormalOrder,		// rowColumnExchange
		                                            MemoryAccessControlNormalOrder,		// verticalRefreshOrder
		                                            MemoryAccessControlColorOrderBGR,	// colorOrder
		                                            MemoryAccessControlNormalOrder);	// horizontalRefreshOrder

  lcdLandscapeMirrorConfig = lcdBuildMemoryAccessControlConfig(
                                                    MemoryAccessControlReverseOrder,	// rowAddressOrder
                                                    MemoryAccessControlReverseOrder,	// columnAddressOrder
                                                    MemoryAccessControlReverseOrder,	// rowColumnExchange
                                                    MemoryAccessControlNormalOrder,		// verticalRefreshOrder
                                                    MemoryAccessControlColorOrderBGR,	// colorOrder
                                                    MemoryAccessControlNormalOrder);	// horizontalRefreshOrder

  lcdReset();

  lcdWriteCommand(ILI9341_DISPLAYOFF);

  lcdWriteCommand(0xCF); //power control B
  lcdWriteData(0x00);
  lcdWriteData(0x83);
  lcdWriteData(0x30);

  lcdWriteCommand(0xED); //power on sequence
  lcdWriteData(0x64);
  lcdWriteData(0x03);
  lcdWriteData(0x12);
  lcdWriteData(0x81);

  lcdWriteCommand(0xE8); //driver timing control
  lcdWriteData(0x85);
  lcdWriteData(0x01);
  lcdWriteData(0x79);

  lcdWriteCommand(0xCB); //power control A
  lcdWriteData(0x39);
  lcdWriteData(0x2C);
  lcdWriteData(0x00);
  lcdWriteData(0x34);
  lcdWriteData(0x02);

  lcdWriteCommand(0xF7); //pump ratio control
  lcdWriteData(0x20);

  lcdWriteCommand(0xEA); //driver timing control B
  lcdWriteData(0x00);
  lcdWriteData(0x00);

  lcdWriteCommand(ILI9341_POWERCONTROL1);
  lcdWriteData(0x26);

  lcdWriteCommand(ILI9341_POWERCONTROL2);
  lcdWriteData(0x11);

  lcdWriteCommand(ILI9341_VCOMCONTROL1);
  lcdWriteData(0x35);
  lcdWriteData(0x3E);

  lcdWriteCommand(ILI9341_VCOMCONTROL2);
  lcdWriteData(0xBE);

  lcdWriteCommand(ILI9341_MEMCONTROL);
  lcdWriteData(lcdLandscapeConfig);

  lcdWriteCommand(ILI9341_PIXELFORMAT);
  lcdWriteData(0x55); //01010101 , 16 bits, 

  lcdWriteCommand(ILI9341_FRAMECONTROLNORMAL);
  lcdWriteData(0x00); //70fps
  lcdWriteData(0x1B);

  lcdWriteCommand(0xF2); //3 gama control
  lcdWriteData(0x08);

  lcdWriteCommand(ILI9341_GAMMASET);
  lcdWriteData(0x01);

  lcdWriteCommand(ILI9341_POSITIVEGAMMCORR);
  lcdWriteData(0x1F);
  lcdWriteData(0x1A);
  lcdWriteData(0x18);
  lcdWriteData(0x0A);
  lcdWriteData(0x0F);
  lcdWriteData(0x06);
  lcdWriteData(0x45);
  lcdWriteData(0x87);
  lcdWriteData(0x32);
  lcdWriteData(0x0A);
  lcdWriteData(0x07);
  lcdWriteData(0x02);
  lcdWriteData(0x07);
  lcdWriteData(0x05);
  lcdWriteData(0x00);

  lcdWriteCommand(ILI9341_NEGATIVEGAMMCORR);
  lcdWriteData(0x00);
  lcdWriteData(0x25);
  lcdWriteData(0x27);
  lcdWriteData(0x05);
  lcdWriteData(0x10);
  lcdWriteData(0x09);
  lcdWriteData(0x3A);
  lcdWriteData(0x78);
  lcdWriteData(0x4D);
  lcdWriteData(0x05);
  lcdWriteData(0x18);
  lcdWriteData(0x0D);
  lcdWriteData(0x38);
  lcdWriteData(0x3A);
  lcdWriteData(0x1F);

  lcdWriteCommand(ILI9341_COLADDRSET); // 0 - 240
  lcdWriteData(0x00);
  lcdWriteData(0x00);
  lcdWriteData(0x00);
  lcdWriteData(0xEF);

  lcdWriteCommand(ILI9341_PAGEADDRSET); // 0 - 320
  lcdWriteData(0x00);
  lcdWriteData(0x00);
  lcdWriteData(0x01);
  lcdWriteData(0x3F);

  lcdWriteCommand(ILI9341_ENTRYMODE);
  lcdWriteData(0x07);

  lcdWriteCommand(ILI9341_DISPLAYFUNC);
  lcdWriteData(0x0A);
  lcdWriteData(0x82);
  lcdWriteData(0x27);
  lcdWriteData(0x00);

  lcdWriteCommand(ILI9341_SLEEPOUT);
  delay(100);
  lcdWriteCommand(ILI9341_DISPLAYON);
  delay(100);
  lcdWriteCommand(ILI9341_MEMORYWRITE);
  
}

void lcdClear(short c){
	unsigned int x, y;
	lcdWriteCommand(0x02c); // write_memory_start
	digitalWrite(LCD_RS, HIGH);
	digitalWrite(LCD_CS, LOW);   
	SetWindows(0, 0, 319, 239); // 479, 319);
	for (x = 0; x < 320; x++)
		for (y = 0; y < 240; y++) {
		  lcdWriteData(c>>8); 
		  lcdWriteData(c); 
		}
	digitalWrite(LCD_CS, HIGH);
}

void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  lcdWriteCommand(0x2a); // Set_column_address 4 parameters
  lcdWriteData(x1 >> 8);   
  lcdWriteData(x1);   
  lcdWriteData(x2 >> 8);  
  lcdWriteData(x2);   
  lcdWriteCommand(0x2b); // Set_page_address 4 parameters
  lcdWriteData(y1 >> 8);   
  lcdWriteData(y1);   
  lcdWriteData(y2 >> 8);   
  lcdWriteData(y2);   
  lcdWriteCommand(0x2c); // Write_memory_start
}

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]){  
  lcdWriteCommand(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+width;
  y2 = y+height;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = 0;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      lcdWriteData( bitmap[k]);
      lcdWriteData( bitmap[k+1] );
      k = k + 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}

void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset){
  lcdWriteCommand(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 

  unsigned int x2, y2;
  x2 =   x+width;
  y2=    y+height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
		for (int j = 0; j < height; j++){
			k = (j*(ancho) + index*width -1 - offset)*2;
			k = k+width*2;
			for (int i = 0; i < width; i++){
				lcdWriteData( bitmap[k]);
				lcdWriteData( bitmap[k+1] );
				k = k - 2;
			} 
		}
  }
  else{
		for (int j = 0; j < height; j++){
			k = (j*(ancho) + index*width + 1 + offset)*2;
			for (int i = 0; i < width; i++){
				lcdWriteData( bitmap[k]);
				lcdWriteData( bitmap[k+1] );
				k = k + 2;
			} 
		}
    
    
  }
  digitalWrite(LCD_CS, HIGH);
}


void LCD_Sprite_Zoom(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char Zoom, int colorfondo){
  lcdWriteCommand(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  int color_h, color_l;
  color_h = 0x00FF & (colorfondo >> 8);
  color_l = 0x00FF & (colorfondo);
  
  unsigned int x2, y2;
  x2 =   x+2*width;
  y2=    y+2*height;
  SetWindows(x, y, x2-1, y2-1);
  int k = 0;
  int ancho = ((width*columns));
  if(flip){
  for (int j = 0; j < height*2; j++){
      k = ((j/2)*(ancho) + index*width - 1 )*2;
      k = k+width*2;
     for (int i = 0; i < width; i++){
        if((bitmap[k] == 0x3a) || (bitmap[k] == 0xe5)){
          lcdWriteData(color_h);
          lcdWriteData(color_l);
          lcdWriteData(color_h);
          lcdWriteData(color_l);
        }
        else{
          lcdWriteData( bitmap[k]);
          lcdWriteData( bitmap[k+1] );
          lcdWriteData( bitmap[k]);
          lcdWriteData( bitmap[k+1] );
        }
      k = k - 2;
     } 
  }
  }else{
     for (int j = 0; j < height*2; j++){
      k = ((j/2)*(ancho) + index*width + 1 )*2;
     for (int i = 0; i < width; i++){
        if((bitmap[k] == 0x3a) || (bitmap[k] == 0xe5)){
          lcdWriteData(color_h);
          lcdWriteData(color_l);
          lcdWriteData(color_h);
          lcdWriteData(color_l);
        }
        else{
          lcdWriteData( bitmap[k]);
          lcdWriteData( bitmap[k+1] );
          lcdWriteData( bitmap[k]);
          lcdWriteData( bitmap[k+1] );
        }
      k = k + 2;
     } 
  }
    
    
    }
  digitalWrite(LCD_CS, HIGH);
}


void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i, j;
  lcdWriteCommand(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
      lcdWriteData(c>>8);
      lcdWriteData(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Funci?n para dibujar una l?nea vertical - par?metros ( coordenada x, cordenada y, longitud, color)
//*************************************************************************************************************************************** 
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {  
  unsigned int i,j;
  lcdWriteCommand(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    lcdWriteData(c>>8);
    lcdWriteData(c);
  }
  digitalWrite(LCD_CS, HIGH);  
}
//***************************************************************************************************************************************
// Funci?n para dibujar un rect?ngulo - par?metros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  lcdWriteCommand(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW); 
  
  unsigned int x2, y2;
  x2 = x+w;
  y2 = y+h;
  SetWindows(x, y, x2-1, y2-1);
  unsigned int k = w*h*2-1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      lcdWriteData(c>>8);
      lcdWriteData(c);
      k = k - 2;
     } 
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Funci?n para dibujar texto - par?metros ( texto, coordenada x, cordenada y, color, background) 
//***************************************************************************************************************************************
void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;
  
  if(fontSize == 1){
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if(fontSize == 2){
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }
  
  char charInput ;
  int cLength = text.length();
  Serial.println(cLength,DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength+1];
  text.toCharArray(char_array, cLength+1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1){
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2){
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
		lcdWriteData(c>>8);
        lcdWriteData(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}