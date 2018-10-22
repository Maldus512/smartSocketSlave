/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include "adc.h"
#include <math.h>

#define ACQ_US_DELAY 5
#define _XTAL_FREQ  32000000UL
/*
double sqrt(double num)
{
    double guess, e, upperbound;
    guess = 1;
    e = 0.001;
    do 
    {
        upperbound = num / guess;
        guess = (upperbound + guess) / 2;
    } while (!(guess * guess >= num - e && 
               guess * guess <= num + e));
    return guess;
}*/

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void initADC() {
    PORTCbits.RC3 = 1;
    ANSELCbits.ANSC3 = 1;
    
    /* Select RC3 as ADC channel */
    ADCON0bits.CHS = 0b010011;
    __delay_ms(2);
    
    /* Voltage reference connected to VDD */
    ADCON1bits.ADPREF = 0b00;
    
    /* Conversion clock = FOSC/32 */
    ADCON1bits.ADCS = 0b010;
    
    /* Right-justified result */
    ADCON1bits.ADFM = 1;
    
    /* Enable ADC module */
    ADCON0bits.ADON = 1;
}


uint16_t readADC() {
    /* Select RC3 as ADC channel */
//    ADCON0bits.CHS = 0b010011;
//    
//    __delay_us(10);
    ADCON0bits.GOnDONE = 1;
    
    while(ADCON0bits.GOnDONE) {
        ;
    }
    
    return (ADRESH << 8) | ADRESL;
}


double currentRead(uint16_t calibration) {
  int rVal = 0;
  int value;
  int sampleCount = 0;
  unsigned long rSquaredSum = 0;
  int rZero = calibration;// 532;//calibration;                // For illustrative purposes only - should be measured to calibrate sensor.
  int counter = 0;
  
  while(counter++ < 100)
  {
      value = (int)readADC();
    rVal = value - rZero;
    rSquaredSum += rVal * rVal;
    sampleCount++;
    __delay_ms(1);
  }
  
  //double tmp = sqrt(rSquaredSum/sampleCount);
  //return tmp;

  double voltRMS = 5.0 * sqrt(rSquaredSum / sampleCount) / 1024.0;

  // x 1000 to convert volts to millivolts
  // divide by the number of millivolts per amp to determine amps measured
  // the 20A module 100 mv/A (so in this case ampsRMS = 10 * voltRMS
  double ampsRMS = voltRMS * 5.405;
  return ampsRMS;
}