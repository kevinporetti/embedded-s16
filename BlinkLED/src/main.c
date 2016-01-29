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

#define DELAY 5000000

int main()
{
   	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
   	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED|LED_BLUE|LED_GREEN);

	//int i = 0;
	while(1)
   	{ 
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, LED_RED|LED_GREEN|LED_BLUE);
		//for(i = 0; i < 5000000; i++);
		ROM_SysCtlDelay(DELAY);
		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED|LED_GREEN|LED_BLUE, 0);
		//for(i = 0; i < 5000000; i++);		
		ROM_SysCtlDelay(DELAY);		
	}
}
