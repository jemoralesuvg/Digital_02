#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

uint8_t greenLedState = HIGH;
uint8_t redLedState = LOW;

void Timer0IntHandler();
void Timer1IntHandler();

void setup()
{
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  
  MAP_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  MAP_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  MAP_TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
  
  Serial.println("hola");
  float period_0 = 0.5; // 200ms
  float period_1 = 0.2; // 200ms
  
  MAP_TimerLoadSet(TIMER0_BASE, TIMER_A, MAP_SysCtlClockGet()*period_0); //(80MHZ / MAP_SysCtlClockGet)
  MAP_TimerLoadSet(TIMER1_BASE, TIMER_A, MAP_SysCtlClockGet()*period_1); //5 veces por seg

  TimerIntRegister(TIMER0_BASE, TIMER_A, &Timer0IntHandler);
  TimerIntRegister(TIMER1_BASE, TIMER_A, &Timer1IntHandler);
  
  MAP_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  MAP_TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  
  MAP_TimerEnable(TIMER0_BASE, TIMER_A);
  MAP_TimerEnable(TIMER1_BASE, TIMER_A);
}

void loop()
{
  // Nothing to do 
}

void Timer0IntHandler() {
  MAP_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  digitalWrite(GREEN_LED, greenLedState);
  greenLedState = !greenLedState;
}

void Timer1IntHandler() {
  MAP_TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  digitalWrite(RED_LED, redLedState);
  redLedState = !redLedState;
  Serial.println(millis());
}
