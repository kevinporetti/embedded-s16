/**
 *	Created by Kevin Poretti
 *	Feb 9th 2016
 */

// Fade method (only one can be defined)
#define REG_FADE
//#define EXP_SCALE_FADE

// STD includes
#include <stdint.h>
#include <stdbool.h>
#ifdef EXP_SCALE_FADE
#include <math.h>
#endif

// Hardware Includes
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

// Tivaware API includes
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

// Macros for on board RGB LED
#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

#ifdef EXP_SCALE_FADE
#define FACTOR 17.33	// Factor used in exp scale calculation
#define INTERVALS 100	// Num of interval in exponential scale
#endif

#define MAX 320			// Max period in clock ticks

/**
 * Delays system in milliseconds
 *
 * @param[in] ms number of milliseconds to delay
 */
void delayMS(int ms)
{
    SysCtlDelay((SysCtlClockGet()/(3*1000))*ms) ;
}

/**
 * Fade an RGB LED using various methods
 */
void main(void)
{
	// Set the clock
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	// Set the PWM Clock
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    // Enable the GPIO port that is used for the on-board LED.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Enable the PWM peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

    /**
     * Sets GPIO pins associated with RGB LED to use PWM peripheral
     * PF1 (RED LED) 	-> M1PWM5
     * PF2 (BLUE LED) 	-> M1PWM6
     * PF3 (GREEN LED)	-> M1PWM7
     */
    GPIOPinConfigure(GPIO_PF1_M1PWM5);
    GPIOPinConfigure(GPIO_PF2_M1PWM6);
    GPIOPinConfigure(GPIO_PF3_M1PWM7);
    GPIOPinTypePWM(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN);

    // Condifure both PWM generators (PWM_GEN_2 -> PWM_OUT_5; PWM_GEN_2 -> PWM_OUT_6 & 7)
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);

    // Set period to MAX clock ticks
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, MAX);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, MAX);

    // Enable PWM generation
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    PWMGenEnable(PWM1_BASE, PWM_GEN_3);

    // Set PWM to output to PWM_OUT_5, 6, and 7
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT | PWM_OUT_6_BIT | PWM_OUT_7_BIT, true);

#ifdef EXP_SCALE_FADE
    float red = 0.0;
    float blue = 0.0;
    float green = 0.0;
#else
    uint32_t red = 0;
    uint32_t blue = 0;
    uint32_t green = 0;
#endif

    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blue);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, green);

    while(1)
    {
#ifdef EXP_SCALE_FADE
    	int i;
    	// off to green
    	for(i = 0; i <= INTERVALS; i++)
		{
    	    green = expf(i/FACTOR) - 1;
    		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, green);
			delayMS(10);
		}
    	// green to teal
    	for(i = 0; i <= INTERVALS; i++)
    	{
    		blue = expf(i/FACTOR) - 1;
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blue);
			delayMS(10);
		}
    	// teal to white
    	for(i = 0; i <= INTERVALS; i++)
		{
    		red = expf(i/FACTOR) - 1;
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red);
			delayMS(10);
		}
    	// white to purple
    	for(i = INTERVALS; i > 0; i--)
		{
    		green = expf(i/FACTOR) - 1;
    		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, green);
			delayMS(10);
		}
    	// purple to red
    	for(i = INTERVALS; i > 0; i--)
		{
    		blue = expf(i/FACTOR) - 1;
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blue);
			delayMS(10);
		}
    	// red to off
    	for(i = INTERVALS; i > 0; i--)
		{
    		red = expf(i/FACTOR) - 1;
			PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red);
			delayMS(10);
		}
#else
    	// off to red
    	for(red = 0; red < MAX; red++)
    	{
    	    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red);
    	    delayMS(5);
    	}
    	// red to purple
    	for(blue = 0; blue < MAX; blue++)
    	{
    	    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blue);
    	    delayMS(5);
    	}
    	// purple to white
    	for(green = 0; green < MAX; green++)
    	{
    		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, green);
    		delayMS(5);
    	}
    	// white to teal
    	for(red = MAX; red > 0; red--)
    	{
    		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, red);
    		delayMS(5);
    	}
    	// teal to green
    	for(blue = MAX; blue > 0; blue--)
    	{
    	    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, blue);
    	    delayMS(5);
    	}
    	// green to off
    	for(green = MAX; green > 0; green--)
    	{
    	    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, green);
    	    delayMS(5);
    	}
#endif
    }
}
