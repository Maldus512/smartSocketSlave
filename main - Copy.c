/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */


#include <xc.h>
#include "stdbool.h"

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


void PIN_MANAGER_Initialize(void)
{
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC2 = 0;

    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC2 = 0;

    LATCbits.LATC2 = 0;
 
    /* disable weak pull up for PORTC */
    WPUC = 0x00;

    /* disable open drain for PORTC */
    ODCONC = 0x00;

    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

//    RX1DTPPSbits.RX1DTPPS = 0x14;   //RC4->EUSART1:RX1;
    
    RC4PPS = 0x0F;   //RA4->EUSART1:TX1;

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS

}       


void OSCILLATOR_Initialize(void)
{
    // NOSC HFINTOSC; NDIV 1;
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power;
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled;
    OSCEN = 0x00;
    // HFFRQ 32_MHz;
    OSCFRQ = 0x06;
    // MFOR not ready;
    OSCSTAT = 0x00;
    // HFTUN 0;
    OSCTUNE = 0x00;
}

void EUSART1_Initialize(void)
{
    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled;
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled;
//    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN disabled; SYNC asynchronous; BRGH hi_speed; CSRC slave;
//    TX1STA = 0x24;

    // Baud Rate = 9600; SP1BRGL 64;
    SP1BRGL = 0x40;

    // Baud Rate = 9600; SP1BRGH 3;
    SP1BRGH = 0x03;
    
    TX1STAbits.TXEN = 1;
    TX1STAbits.SYNC = 0;
    TX1STAbits.TX9 = 0;
    TX1STAbits.BRGH = 1;
    RC1STAbits.SPEN = 1;
}


void EUSART1_Write(uint8_t txData)
{
    while(0 == PIR3bits.TX1IF)
    {
    }
    TX1REG = txData;    // Write the data byte to the USART.
}


void SYSTEM_Initialize(void)
{

    PIN_MANAGER_Initialize();
//    OSCILLATOR_Initialize();
    EUSART1_Initialize();
    initTimer();
}




void main(void)
{
    // initialize the device
    SYSTEM_Initialize();

    while (1)
    {
        EUSART1_Write('A');
//        EUSART1_Write(0x44);
//        EUSART1_Write(0x75);
//        EUSART1_Write(0x73);
//        EUSART1_Write(0x74);
//        EUSART1_Write(10);
//        EUSART1_Write(13);

    }
}


///******************************************************************************/
///* User Global Variable Declaration                                           */
///******************************************************************************/
//void EUSART1_Write(uint8_t txData);
///* i.e. uint8_t <variable_name>; */
//
///******************************************************************************/
///* Main Program                                                               */
///******************************************************************************/
//void main(void)
//{
//    /* Configure the oscillator for the device */
//    ConfigureOscillator();
//
//    /* Initialize I/O and Peripherals for application */
//    InitApp();
//
//
//    while(1)
//    {
//        int x = 0, y =0;
//        for (x =0; x < 1000; x++)
//            y++;
//        
//        EUSART1_Write('a');
//
//
//        /* TODO <INSERT USER APPLICATION CODE HERE> */
//    }
//
//}
//
