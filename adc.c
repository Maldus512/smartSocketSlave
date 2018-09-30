/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "adc.h"
#include <stdint.h>

#define ACQ_US_DELAY 5
#define _XTAL_FREQ  32000000UL



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
    
    /* Conversion clock = FOSC/2 */
    ADCON1bits.ADCS = 0b110;
    
    /* Right-justified result */
    ADCON1bits.ADFM = 1;
    
    /* Enable ADC module */
    ADCON0bits.ADON = 1;
}


uint16_t readADC() {
    /* Select RC3 as ADC channel */
    ADCON0bits.CHS = 0b010011;
    
    __delay_ms(2);
    ADCON0bits.GOnDONE = 1;
    
    while(ADCON0bits.GOnDONE) {
        ;
    }
    
    return (ADRESH << 8) | ADRESL;
}
