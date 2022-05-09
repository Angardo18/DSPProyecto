

/**
 * main.c
 */
#include  <stdint.h>
#include <stdbool.h>

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
//----------- utilidades -------------------
#include "uartstdio.h"

uint16_t cont = 0;
int main(void){
    // Frecuencia de operacion a 80 MHZ
    SysCtlClockSet(SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN
                   | SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL);
    //---------- PUERTO UART ------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); //para habilitar el puertoA
    GPIOPinConfigure(GPIO_PA0_U0RX); //configuracion del periferico al que estaran
    GPIOPinConfigure(GPIO_PA1_U0TX);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0); //habilitar el puerto
    // usar el oscilador interno de 16MHz para el uart
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 1000000, 16000000);

    while(1){
        UARTprintf("%d\n",(cont++)%128);
        SysCtlDelay(200000);
    }

	return 0;
}
