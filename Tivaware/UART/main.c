//**************************************************************************************************************
/**
 * main.c
 * Código ejemplo de uso de Interrupciones y de timer
 */
//**************************************************************************************************************
// Librerias
//**************************************************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"

#define XTAL 16000000

//**************************************************************************************************************
// Variables Globales
//**************************************************************************************************************

//**************************************************************************************************************
// Prototipos de Función
//**************************************************************************************************************
void InitUART(void);
//**************************************************************************************************************
// Código Principal
//**************************************************************************************************************
int main(void)
{
    // Se setea oscilador externo de 16MHz
    SysCtlClockSet(
    SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // Se inicializa la comunicación UART
    InitUART();

    // Se asigna reloj a puerto F
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Se establecen como salidas los pines 1, 2 y 3 del puerto F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
    GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Se manda mensajes por UART
    UARTCharPut(UART0_BASE, 'H');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 'l');
    UARTCharPut(UART0_BASE, 'a');
    UARTCharPut(UART0_BASE, ' ');
    UARTCharPut(UART0_BASE, 'M');
    UARTCharPut(UART0_BASE, 'u');
    UARTCharPut(UART0_BASE, 'n');
    UARTCharPut(UART0_BASE, 'd');
    UARTCharPut(UART0_BASE, 'o');
    UARTCharPut(UART0_BASE, 10);
    UARTCharPut(UART0_BASE, 13);
    //**********************************************************************************************************
    // Loop Principal
    //**********************************************************************************************************
    while (1)
    {

    }
}
//**************************************************************************************************************
// Inicialización de UART
//**************************************************************************************************************
void InitUART(void)
{
    /*Enable the GPIO Port A*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    /*Enable the peripheral UART Module 0*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    /* Make the UART pins be peripheral controlled. */
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(
            UART0_BASE, SysCtlClockGet(), 115200,
            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    //UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}

