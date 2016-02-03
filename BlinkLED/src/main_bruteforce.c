#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

#define DELAY 500000

int main()
{
   	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
   	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED);

	int i = 0;
	while(1)
   	{ 
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, LED_RED);
		for(i = 0; i < DELAY; i++);
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, 0);
		for(i = 0; i < DELAY; i++);		
	}
}
