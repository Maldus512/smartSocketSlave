/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */
#include "hardware.h"
#include "system.h"

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits->Oscillator not enabled
#pragma config RSTOSC = HFINT32    // Power-up default value for COSC bits->HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1
#pragma config CLKOUTEN = OFF    // Clock Out Enable bit->CLKOUT function is disabled; i/o or oscillator function on OSC2
#pragma config CSWEN = OFF    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable bit->FSCM timer enabled

// CONFIG2
#pragma config MCLRE = ON    // Master Clear Enable bit->MCLR pin is Master Clear function
#pragma config PWRTE = OFF    // Power-up Timer Enable bit->PWRT disabled
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit->ULPBOR disabled
#pragma config BOREN = ON    // Brown-out reset enable bits->Brown-out Reset Enabled, SBOREN bit is ignored
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices
#pragma config ZCD = OFF    // Zero-cross detect disable->Zero-cross detect circuit is disabled at POR.
#pragma config PPS1WAY = OFF    // Peripheral Pin Select one-way control->The PPSLOCK bit can be set and cleared repeatedly by software
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a reset

// CONFIG3
#pragma config WDTCPS = WDTCPS_31    // WDT Period Select bits->Divider ratio 1:65536; software control of WDTPS
#pragma config WDTE = OFF    // WDT operating mode->WDT Disabled, SWDTEN is ignored
#pragma config WDTCWS = WDTCWS_7    // WDT Window Select bits->window always open (100%); software control; keyed access not required
#pragma config WDTCCS = SC    // WDT input clock selector->Software Control

// CONFIG4
#pragma config BBSIZE = BB512    // ->512 words boot block size
#pragma config BBEN = OFF    // ->Boot Block disabled
#pragma config SAFEN = OFF    // ->SAF disabled
#pragma config WRTAPP = OFF    // ->Application Block not write protected
#pragma config WRTB = OFF    // ->Boot Block not write protected
#pragma config WRTC = OFF    // ->Configuration Register not write protected
#pragma config WRTSAF = OFF    // ->SAF not write protected
#pragma config LVP = OFF    // Low Voltage Programming Enable bit->High Voltage on MCLR/Vpp must be used for programming

// CONFIG5
#pragma config CP = OFF    // UserNVM Program memory code protection bit->UserNVM code protection disabled

/* Refer to the device datasheet for information about available
oscillator configurations. */
//void ConfigureOscillator(void)
//{
//    /* TODO Add clock switching code if appropriate.  */
//
//    /* Typical actions in this function are to tweak the oscillator tuning
//    register, select new clock sources, and to wait until new clock sources
//    are stable before resuming execution of the main project. */
//
//    // NOSC HFINTOSC; NDIV 1;
//    OSCCON1 = 0x60;
//    // CSWHOLD may proceed; SOSCPWR Low power;
//    OSCCON3 = 0x00;
//    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled;
//    OSCEN = 0x00;
//    // HFFRQ 32_MHz;
//    OSCFRQ = 0x06;
//    // MFOR not ready;
//    OSCSTAT = 0x00;
//    // HFTUN 0;
//    OSCTUNE = 0x00;
//
//}




void initGPIO()
{
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 1;
    RELAY_TRIS = 0;
    TRISCbits.TRISC1 = 1;

    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC5 = 0;
    ANSELCbits.ANSC2 = 0;
    ANSELCbits.ANSC1 = 0;
    
    NOTIFY_TRIS = 0;

    NOTIFY = 0;
    RELAY = 0;
 
    /* disable weak pull up for PORTC */
    WPUC = 0x00;

    /* disable open drain for PORTC */
    ODCONC = 0x00;
    
    
    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    RX1DTPPSbits.RX1DTPPS = 0x15;   //RC5->EUSART1:RX1;
    
    RC4PPS = 0x0F;   //RC4->EUSART1:TX1;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS

}      




void initTimer() {
    
    /* Low Frequency internal oscillator, 31 kHz*/
    T0CON1bits.T0CS = 0b100;
    /* Prescaler 1:1 */
    T0CON1bits.T0CKPS = 0b0000;
    T0CON1bits.T0ASYNC = 1;
    T0CON1bits.T0CKPS = 0b0000;
    T0CON0bits.T016BIT = 0;

    /* By running 31 times the timer should be 1 ms */
    
    
    T0CON0bits.T0EN = 1;
    
    PIE0bits.TMR0IE = 1;
    INTCONbits.GIE = 1;
    
    TMR0H = 31-1;
    
}
