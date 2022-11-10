#include "TM4C123.h"                    // Device header
#include <stdint.h>

#define RED (1U << 1)
#define GREEN (1U << 3)
#define BLUE (1U << 2)
#define R5 (1U << 5)

#define XTAL 16000000

uint32_t clock1ms = XTAL/1000;
	
void delay_ms(uint32_t ms);
void initTimer0_OneShot(uint32_t mdelay);

int main(void){
	
	SYSCTL->RCGCGPIO |= R5;
	GPIOF->DIR |= RED | GREEN | BLUE;
	GPIOF->DEN |= RED | GREEN | BLUE;
	
	while(1){
		GPIOF->DATA |= RED;
		//delay_ms(500);
		initTimer0_OneShot(500);
		GPIOF->DATA &= ~RED;
		//delay_ms(500);
		initTimer0_OneShot(500);
	}	

}
void initTimer0_OneShot(uint32_t mdelay){
	SYSCTL->RCGCTIMER |= 0x01;
	TIMER0->CTL |= 0x00;
	TIMER0->CFG = 0x00;			// 32-bits
	TIMER0->TAMR = 0x01;		// one-shot
	TIMER0->TAILR = clock1ms * mdelay -1;		// Establecer el rango de tiempo
	TIMER0->ICR = 0x01;
	TIMER0->CTL |= 0x01;
	while((TIMER0->RIS & 0x01) == 0);		// Esperar a que se encienda la bandera de timeout
	
}
void delay_ms(uint32_t ms){
	SysTick->LOAD = clock1ms*ms - 1;
	SysTick->CTRL = (1U << 2)|(1U);  // Clock Source: System Clock | Enable  
	while(!(SysTick->CTRL & (1U << 16))){}  //Bit Count = 1? The SysTick timer has counted to 0 since the last time
}
