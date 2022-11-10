#include "TM4C123GH6PM.h"

/*
#define SYSCTL_RCGCGPIO_R (*((unsigned int *) 0x400FE608U))
#define GPIO_PORTF_DIR_R (*((unsigned int *) 0x40025400U))
#define GPIO_PORTF_DEN_R (*((unsigned int *) 0x4002551CU))
#define GPIO_PORTF_DATA_R (*((unsigned int *) 0x400253FCU))
*/

int delay;

int main(){
	SYSCTL_RCGCGPIO_R = 0x20U;
	GPIO_PORTF_DIR_R = 0x0EU;
	GPIO_PORTF_DEN_R = 0x0EU;
	
	while(1){
		delay = 0;
		while(delay < 1000000){
			GPIO_PORTF_DATA_R = 0x0EU;
			++delay;
		}
		
		delay = 0;
		
		while(delay < 1000000){
			GPIO_PORTF_DATA_R = 0x00U;
			++delay;
		}
		
	}
	
}