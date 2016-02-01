#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"
#include "driverlib/timer.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

int main()
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
   	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
   	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_SysCtlPeripheralReset(SYSCTL_PERIPH_TIMER0);
	ROM_TimerDisable(TIMER0_BASE, TIMER_A);
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC);
	ROM_TimerPrescaleSet(TIMER0_BASE, TIMER_A, 0xFFFF);

	uint32_t timer_value = 0;
	ROM_TimerEnable(TIMER0_BASE, TIMER_A);

	uint32_t led_value = 0;
	ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, led_value);
	
	while(1)
   	{ 
		timer_value = ROM_TimerValueGet(TIMER0_BASE, TIMER_A);
		if(timer_value)
		{
			led_value ^= LED_GREEN;
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, led_value);
			
			timer_value = 0;
		}
	}
}
