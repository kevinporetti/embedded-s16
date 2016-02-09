/**
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

#define MAX 500

/**
 * Fade an RGB LED using various methods
 */
int
main(void)
{
	// Set the clock
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	// Set the PWM Clock
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	//
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    /**
     * PF1 (RED LED) 	-> M1PWM5
     * PF2 (GREEN LED) 	-> M1PWM6
     * PF3 (BLUE LED)	-> M1PWM7
     */
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE);

    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);

    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, MAX);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, MAX);

    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);

    //
    // We are finished.  Hang around doing nothing.
    //
    uint32_t red = MAX;
    uint32_t green = MAX;
    uint32_t blue = MAX;

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, green);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, blue);

    while(1)
    {
    	for(blue = MAX; blue > 0; blue--)
    	{
    		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, blue);
    	}

    	SysCtlDelay(200);

    	for(green = MAX; green > 0; green--)
    	{
    		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, green);
    	}

    	SysCtlDelay(200);

    	for(red = MAX; red > 0; red--)
    	{
    		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red);
    	}
    }
}
