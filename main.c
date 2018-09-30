#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include "system.h"
#include "uart.h"
#include "interrupts.h"
#include "adc.h"

#define _XTAL_FREQ  32000000UL
#include <pic.h>
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void initSystem(void)
{
//    ConfigureOscillator();
    initGPIO();
    initTimer();
    initUART();
    initADC();
}


int main(void)
{
    char res[32];
    // initialize the device
    initSystem();
    
    
    while (1)
    {
        
        sprintf(res, "read = %u\n\r", readADC());
        UARTBlockingWrite(res, strlen(res));
        __delay_ms(500);
        //ac_read();
    }
}
