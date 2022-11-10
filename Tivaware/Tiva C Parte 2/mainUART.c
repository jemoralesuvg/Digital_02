#include "TM4C123.h"                    // Device header
#include <stdint.h>

#define RED (1U << 1)
#define GREEN (1U << 3)
#define BLUE (1U << 2)

#define RPF (1U << 5)

#define XTAL 16000000

uint32_t clock1ms = XTAL/1000;
char c;

void delay_ms(uint32_t ms);
void initUART(void);
char UART_Read(void);
void UART_Write(char c);
void UART_Write_String(char * string);	

int main(void){
	
	
	SYSCTL->RCGCGPIO |= RPF;
	GPIOF->DIR |= RED | GREEN | BLUE;
	GPIOF->DEN |= RED | GREEN | BLUE;
	
	initUART();
	delay_ms(200);
	UART_Write('H');
	UART_Write('o');
	UART_Write('l');
	UART_Write('a');
	UART_Write('\n');
	UART_Write('\r');
	
	while(1){
		UART_Write_String("Enter r, g, or b:\n\r");
		c = UART_Read();
		
		UART_Write(c);
		UART_Write_String("\n\r");
		switch(c){
			case 'r':
				GPIOF->DATA = RED;
				break;
			case 'g':
				GPIOF->DATA = GREEN;
				break;
			case 'b':
				GPIOF->DATA = BLUE;
				break;
			default:
				GPIOF->DATA = 0;
				break;
		}
	}

}
void delay_ms(uint32_t ms){
	SysTick->LOAD = clock1ms*ms - 1;
	SysTick->CTRL = (1U << 2)|(1U<<0);  // Clock Source: System Clock | Enable  0b101
	while(!(SysTick->CTRL & (1U << 16))){}  //Bit Count = 1? The SysTick timer has counted to 0 since the last time
}

void initUART(void){
	
	// 1. Enable the UART module using the RCGCUART register (see page 344).
	SYSCTL->RCGC1 |= (1<<0);
	SYSCTL->RCGCUART |= (1<<0);
	
	// 2. Enable the clock to the appropriate GPIO module via the RCGCGPIO register (see page 340).
	// To find out which GPIO port to enable, refer to Table 23-5 on page 1351.
	SYSCTL->RCGCGPIO |= (1<<0);
	
	// 3. Set the GPIO AFSEL bits for the appropriate pins (see page 671). To determine which GPIOs to
	// configure, see Table 23-4 on page 1344.	
	GPIOA->AFSEL = (1<<1)|(1<<0);
	
	// 4. Configure the GPIO current level and/or slew rate as specified for the mode selected (see
	// page 673 and page 681).
	
	// 5. Configure the PMCn fields in the GPIOPCTL register to assign the UART signals to the appropriate
	// pins (see page 688 and Table 23-5 on page 1351).
	GPIOA->DEN = (1<<0)|(1<<1);
	GPIOA->PCTL = (1<<0)|(1<<4);
	
	
	// Find the Baud-Rate Divisor
	// BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
	// where UARTSysClk is the system clock connected to the UART, and ClkDiv is either 16 (if HSE
	// in UARTCTL is clear) or 8 (if HSE is set).
	// UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5)
	
	// Ejemplo 9600 baudrate
	// BRD = 16000000 / (16 * 9600) = 104.1667
	// UARTFBRD[DIVFRAC] = integer(0.1667 * 64 + 0.5) = 11
	
	// With the BRD values in hand, the UART configuration is written to the module in the following order:
	
	// 1. Disable the UART by clearing the UARTEN bit in the UARTCTL register.
	UART0->CTL &= ~(1<<0);
	// 2. Write the integer portion of the BRD to the UARTIBRD register.
	UART0->IBRD = 104;
	
	// 3. Write the fractional portion of the BRD to the UARTFBRD register.
	UART0->FBRD = 11;
	
	// 4. Write the desired serial parameters to the UARTLCRH register
	// no parity | 8 data bits | disable FIFO | 1 stop bit | disable parity | normal use
	UART0->LCRH = (1<<6)|(1<<5);
	
	// 5. Configure the UART clock source by writing to the UARTCC register.
	UART0->CC |= (1<<0);
	
	// 6. Optionally, configure the µDMA channel (see “Micro Direct Memory Access (µDMA)” on page 585)
	// and enable the DMA option(s) in the UARTDMACTL register. 
	
	// 7. Enable the UART by setting the UARTEN bit in the UARTCTL register.
	UART0->CTL |= (1<<0)|(1<<8)|(1<<9);
	
}


char UART_Read(void){
	char c;
	// UART0->FR Flag Register Pg. 911
	while((UART0->FR & (1<<4)) != 0);
	c = UART0->DR;
	return c;
}
void UART_Write(char c){
	// UART0->FR Flag Register Pg. 911
	while((UART0->FR & (1<<5)) != 0);
	UART0->DR = c;
}
void UART_Write_String(char * string){
	while(*string){
		UART_Write(*(string++));
	}
}
