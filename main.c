

/**
 * main.c
 */
#include  <stdint.h>
#include <stdbool.h>

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
//----------- utilidades -------------------
#include "uartstdio.h"
// -- variables de pruebas -----------------
uint16_t cont = 0;

//------- variables globales ---------------
uint16_t freq_muestreo = 1000;// En Hz

//******Funciones de interrupciones*********
void Timer0IntHandler(void){
    //! - ADC0 peripheral
    //! - GPIO Port E peripheral (for AIN0 pin)
    //! - AIN0 - PE3
    uint32_t ADCvalue[1];
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    // Trigger the ADC conversion.
    ADCProcessorTrigger(ADC0_BASE, 3);
    // Wait for conversion to be completed.
    while(!ADCIntStatus(ADC0_BASE, 3, false)){}
    // Clear the ADC interrupt flag.
    ADCIntClear(ADC0_BASE, 3);
    ADCSequenceDataGet(ADC0_BASE, 3, ADCvalue);
    // Display the AIN0 (PE3) digital value on the console.
    UARTprintf("%d\n", ADCvalue[0]);
}



int main(void){
    /*Configuraciones de los perifericos de la tiva*/
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
    //--------------- ADC --------------------------------
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0); //Conectar el adc
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);//habilitar el gpio
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3);//configurar pin como analogico
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 |
                                             ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ADC0_BASE, 3); //habilitar secuencia
    ADCIntClear(ADC0_BASE, 3);
    //----------------- HABILITAR INTERRUPCIONES ----------
    IntMasterEnable();
    //-------- TIMER 0 ------------------------------------
    /*este timer se utilizará para la lectura del ADC   */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, (uint32_t)(SysCtlClockGet()/freq_muestreo));
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A);



    while(1){
        //UARTprintf("%d\n",(cont++)%128);
        SysCtlDelay(200000);
    }

	return 0;
}
