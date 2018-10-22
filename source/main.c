#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include "hardware.h"
#include <stdint.h>        /* For uint8_t definition */
#include "system.h"
#include "uart.h"
#include "interrupts.h"
#include "adc.h"
#include "state.h"


#include <stdio.h>
#include <string.h>


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
    int i;
    char stringa[64];
    command_t command;
    unsigned long average = 0;
    uint16_t calibration = 0;
    
    int calibrationMeanCount = 0;
    // initialize the device
    initSystem();
    
    while (1)
    {
        command = nextCommand();
        
        if (command == ON) {
            RELAY = 1;
            stato.f_relayOn = 1;
        }
        else if (command == OFF) {
            RELAY = 0;
            stato.f_relayOn = 0;
            average = 0;
            calibrationMeanCount = 0;
        }
        else if (command == PRINT_READING) {
            stato.f_transmitSensorReadings = 1;
        }
        else if (command == CALIBRATE) {
            f_calibration = 1;
        }
        
        /*if (stato.f_relayOn == 0) {
            average += readADC();
            
            if (calibrationMeanCount++ == 1000) {
                calibrationMeanCount = 0;
                calibration = average/1000;
                average = 0;
            }
        }*/
        
        if (f_100ms && stato.f_transmitSensorReadings == 1) {
//            button = PORTCbits.RC1;
//            raw = readADC();
//            clean = raw > calibration ? raw-calibration : 0;
            sprintf(stringa, "current = %.3f\n\r", currentRead(calibration));
            UARTBlockingWrite(stringa, strlen(stringa));
            sprintf(stringa, "adc = %i - cal = %i\n\r", readADC(), calibration);
            UARTBlockingWrite(stringa, strlen(stringa));
            f_100ms = 0;
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
            sprintf(stringa, "average found = %u\n\r", calibration);
            UARTBlockingWrite(stringa, strlen(stringa));
        }
        
        __delay_ms(1);
    }
}
