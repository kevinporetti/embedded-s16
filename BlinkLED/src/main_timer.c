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
#include "driverlib/timer.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

int main()
{
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_GREEN);

	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	ROM_TimerDisable(TIMER0_BASE, TIMER_A);
	ROM_TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	ROM_TimerLoadSet(TIMER0_BASE, TIMER_A, ROM_SysCtlClockGet());

	ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, 0);

	ROM_TimerEnable(TIMER0_BASE, TIMER_A);

	while(1)
   	{
		if(ROM_TimerValueGet(TIMER0_BASE, TIMER_A) >= (ROM_SysCtlClockGet()/2))
		{
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, LED_GREEN);
		}
		else
		{
			ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, 0);
		}
	}
}
