#include "TM4C123.h"


#define RED (1U << 1)
#define GREEN (1U << 3)
#define BLUE (1U << 2)

int delay;

int main(){
	
	SYSCTL->RCGCGPIO = 0x20U;
	GPIOF->DIR |= RED | GREEN | BLUE;
	GPIOF->DEN |= RED | GREEN | BLUE;
	
	while(1){
		delay = 0;
		while(delay < 1000000){
			GPIOF->DATA |= RED | GREEN | BLUE;
			++delay;
		}
		
		delay = 0;
		
		while(delay < 1000000){
			GPIOF->DATA &= ((~RED) | (~GREEN) | (~BLUE));
			++delay;
		}
		
	}
	
}