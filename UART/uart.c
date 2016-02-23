#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#define BAUD_RATE 9600

// Macros for on board RGB LED
#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

void
UARTIntHandler(void)
{
	int32_t ui32Status;

	//
	// Get the interrrupt status.
	//
	ui32Status = ROM_UARTIntStatus(UART0_BASE, true);

    ROM_UARTIntClear(UART0_BASE, ui32Status);

    uint8_t command = ROM_UARTCharGetNonBlocking(UART0_BASE);

    switch(command)
    {
    	case 'r':
    	case 'R':
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, LED_RED);
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, 0);
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, 0);
    		break;
    	case 'b':
    	case 'B':
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, 0);
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, LED_BLUE);
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, 0);
    		break;
    	case 'g':
    	case 'G':
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED, 0);
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_BLUE, 0);
    		ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_GREEN, LED_GREEN);
    		break;
    }

}


//*****************************************************************************
//
// Toggle a GPIO.
//
//*****************************************************************************
int
main(void)
{
    //
    // Enable the GPIO module.
    //
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
	// Enable processor interrupts.
	//
	ROM_IntMasterEnable();

   	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN);

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART
    //
    ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), BAUD_RATE,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    ROM_IntEnable(INT_UART0);
    ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

    //
    // Loop forever.
    //
    while(1)
    {
    }
}
