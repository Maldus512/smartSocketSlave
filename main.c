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


uint8_t f_calibration = 0;

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
    char res[64];
    unsigned long average = 0;
    int i;
    uint16_t raw;
    uint16_t calibration = 0;
    uint16_t clean = 0;
    uint8_t button;
    // initialize the device
    initSystem();
    LATCbits.LATC2 = 1;
    
    while (1)
    {
        
        if (f_100ms) {
//            button = PORTCbits.RC1;
//            raw = readADC();
//            clean = raw > calibration ? raw-calibration : 0;
            sprintf(res, "current = %f\n\r", currentRead(calibration));
            UARTBlockingWrite(res, strlen(res));
            f_100ms = 0;
        }
        
        if (PORTCbits.RC1 == 0) {
            __delay_ms(5);
            
            if (PORTCbits.RC1 == 0) {
                f_calibration = 1;
            }
        }
        
        if (f_calibration) {
            UARTBlockingWrite("Begin calibration\n\r", 19);
            average = 0;
            for (i = 0; i < 1000; i++) {
                average += readADC();
                __delay_ms(2);
            }
            
            calibration = average/1000;
            f_calibration = 0;
            
            UARTBlockingWrite("End calibration\n\r", 17);
            sprintf(res, "average found = %u\n\r", calibration);
            UARTBlockingWrite(res, strlen(res));
        }
        
    }
}
