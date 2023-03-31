/*
 * ili9341
 * Autor: Jos? Eduardo Morales
 * Creaci?n: 4/10/2022	Modificaci?n: 4/10/2022
 *	Basado en ili9341_light.c de "tabur"
 */

#ifndef ILI9341_8_BITS_FAST_H_
#define ILI9341_8_BITS_FAST_H_

#include <energia.h>
#include "lcd_registers.h"
#include "font.h"

/*---------------------------------------------------------------------*/
// Register names from Peter Barrett's Microtouch code
#define	ILI9341_NOP					0x00
#define ILI9341_SOFTRESET			0x01
#define ILI9341_READID				0x04
#define ILI9341_READSTATUS			0x09
#define ILI9341_READPOWERMODE		0x0A
#define ILI9341_READMADCTL			0x0B
#define ILI9341_READPIXELFORMAT		0x0C
#define ILI9341_READIMAGEFORMAT		0x0D
#define ILI9341_READSIGNALMODE		0x0E
#define ILI9341_READSELFDIAGNOSTIC	0x0F
#define ILI9341_SLEEPIN				0x10
#define ILI9341_SLEEPOUT			0x11
#define ILI9341_PARTIALMODE			0x12
#define ILI9341_NORMALDISP			0x13
#define ILI9341_INVERTOFF			0x20
#define ILI9341_INVERTON			0x21
#define ILI9341_GAMMASET			0x26
#define ILI9341_DISPLAYOFF			0x28
#define ILI9341_DISPLAYON			0x29
#define ILI9341_COLADDRSET			0x2A
#define ILI9341_PAGEADDRSET			0x2B
#define ILI9341_MEMORYWRITE			0x2C
#define ILI9341_COLORSET			0x2D
#define ILI9341_MEMORYREAD			0x2E
#define ILI9341_PARTIALAREA			0x30
#define ILI9341_VERTICALSCROLING	0x33
#define ILI9341_TEARINGEFFECTOFF	0x34
#define ILI9341_TEARINGEFFECTON		0x35
#define ILI9341_MEMCONTROL			0x36
#define ILI9341_VSCROLLSTARTADDRESS	0x37
#define ILI9341_IDLEMODEOFF			0x38
#define ILI9341_IDLEMODEON			0x39
#define ILI9341_PIXELFORMAT			0x3A
#define ILI9341_WRITEMEMCONTINUE	0x3C
#define ILI9341_READMEMCONTINUE		0x3E
#define ILI9341_SETSCANLINE			0x44
#define ILI9341_GETSCANLINE			0x45
#define ILI9341_WRITEBRIGHTNESS		0x51
#define ILI9341_READBRIGHTNESS		0x52
#define ILI9341_WRITECTRL			0x53
#define ILI9341_READCTRL			0x54
#define ILI9341_WRITECABC			0x55
#define ILI9341_READCABC			0x56
#define ILI9341_WRITECABCMIN		0x5E
#define ILI9341_READCABCMIN			0x5F
#define ILI9341_RGBSIGNALCONTROL	0xB0
#define ILI9341_FRAMECONTROLNORMAL	0xB1
#define ILI9341_FRAMECONTROLIDLE	0xB2
#define ILI9341_FRAMECONTROLPARTIAL	0xB3
#define ILI9341_INVERSIONCONTROL	0xB4
#define ILI9341_BLANKINGPORCHCONT	0xB5
#define ILI9341_DISPLAYFUNC			0xB6
#define ILI9341_ENTRYMODE			0xB7
#define ILI9341_BACKLIGHTCONTROL1	0xB8
#define ILI9341_BACKLIGHTCONTROL2	0xB9
#define ILI9341_BACKLIGHTCONTROL3	0xBA
#define ILI9341_BACKLIGHTCONTROL4	0xBB
#define ILI9341_BACKLIGHTCONTROL5	0xBC
#define ILI9341_BACKLIGHTCONTROL7	0xBE
#define ILI9341_BACKLIGHTCONTROL8	0xBF
#define ILI9341_POWERCONTROL1		0xC0
#define ILI9341_POWERCONTROL2		0xC1
#define ILI9341_VCOMCONTROL1		0xC5
#define ILI9341_VCOMCONTROL2		0xC7
#define ILI9341_NVMEMORYWRITE		0xD0
#define ILI9341_NVMEMORYKEY			0xD1
#define ILI9341_NVMEMORYSTATUSREAD	0xD2
#define ILI9341_READID4				0xD3
#define ILI9341_READID1				0xDA
#define ILI9341_READID2				0xDB
#define ILI9341_READID3				0xDC
#define ILI9341_POSITIVEGAMMCORR	0xE0
#define ILI9341_NEGATIVEGAMMCORR	0xE1
#define ILI9341_DIGITALGAMMCONTROL1	0xE2
#define ILI9341_DIGITALGAMMCONTROL2	0xE3
#define ILI9341_INTERFACECONTROL	0xF6

#define ILI9341_MADCTL_MY			0x80
#define ILI9341_MADCTL_MX			0x40
#define ILI9341_MADCTL_MV			0x20
#define ILI9341_MADCTL_ML			0x10
#define ILI9341_MADCTL_RGB			0x00
#define ILI9341_MADCTL_BGR			0x08
#define ILI9341_MADCTL_MH			0x04

// Color definitions
#define BLACK 0x0000       ///<   0,   0,   0
#define NAVY 0x000F        ///<   0,   0, 123
#define DARKGREEN 0x03E0   ///<   0, 125,   0
#define DARKCYAN 0x03EF    ///<   0, 125, 123
#define MAROON 0x7800      ///< 123,   0,   0
#define PURPLE 0x780F      ///< 123,   0, 123
#define OLIVE 0x7BE0       ///< 123, 125,   0
#define LIGHTGREY 0xC618   ///< 198, 195, 198
#define DARKGREY 0x7BEF    ///< 123, 125, 123
#define BLUE 0x001F        ///<   0,   0, 255
#define GREEN 0x07E0       ///<   0, 255,   0
#define CYAN 0x07FF        ///<   0, 255, 255
#define RED 0xF800         ///< 255,   0,   0
#define MAGENTA 0xF81F     ///< 255,   0, 255
#define YELLOW 0xFFE0      ///< 255, 255,   0
#define WHITE 0xFFFF       ///< 255, 255, 255
#define ORANGE 0xFD20      ///< 255, 165,   0
#define GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define PINK 0xFC18        ///< 255, 130, 198

//class Lcd16
//{
//	public:
	//void lcdSetDataPins(int DPINS[8]);
	void lcdInit(void);
	void lcdClear(short c);
	void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
	void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);  
	void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char offset);
	void LCD_Print(String text, int x, int y, int fontSize, int color, int background);
	void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
	void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
	void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);  
	void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
	void LCD_Sprite_Zoom(int x, int y, int width, int height, unsigned char bitmap[],int columns, int index, char flip, char Zoom, int colorfondo);

/*---------------------------------------------------------------------*/
//}
#endif /* ILI9341_8_BITS_H_ */
