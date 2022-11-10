#include "TM4C123.h"                    // Device header
#include <stdint.h>

#define RED (1U << 1)
#define GREEN (1U << 3)
#define BLUE (1U << 2)
#define R5 (1U << 5)

#define XTAL 16000000

uint32_t clock1ms = XTAL/1000;
	
void delay_ms(uint32_t ms);

int main(void){
	
	SYSCTL->RCGCGPIO |= R5;
	GPIOF->DIR |= RED | GREEN | BLUE;
	GPIOF->DEN |= RED | GREEN | BLUE;
	
	while(1){
		GPIOF->DATA |= BLUE;
		delay_ms(500);
		GPIOF->DATA &= ~BLUE;
		delay_ms(500);
	}

}
void delay_ms(uint32_t ms){
	SysTick->LOAD = clock1ms*ms - 1;
	SysTick->CTRL = (1U << 2)|(1U);  // Clock Source: System Clock | Enable  
	while(!(SysTick->CTRL & (1U << 16))){}  //Bit Count = 1? The SysTick timer has counted to 0 since the last time
}
