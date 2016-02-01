#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

static volatile uint32_t led_value = 0;

void Timer0AIntHandler(void)
{ 
    ROM_TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	led_value ^= LED_GREEN;
	ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, led_value);
}

int main()
{
   	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
   	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_SysCtlPeripheralReset(SYSCTL_PERIPH_TIMER0);
	ROM_TimerDisable(TIMER0_BASE, TIMER_A);
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, 0xFF);

    ROM_IntMasterEnable();
    ROM_IntEnable(INT_TIMER0A);
	ROM_TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);


	led_value = 0;
	ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, led_value);

	ROM_TimerEnable(TIMER0_BASE, TIMER_A);
	while(1)
   	{}
}
