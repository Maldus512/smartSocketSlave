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
#include <string.h>

#include "system.h"
#include "uart.h"


char uart_buffer[UART_BUF];
int uart_index = 0;
int to_transmit = 0;




void initUART() {
    
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
    RC1STAbits.CREN = 1;
    
    INTCONbits.PEIE = 1;
    PIE3bits.RC1IE = 1;
    
}


void UARTputc(char txData)
{
    while(0 == PIR3bits.TX1IF)
    {
    }
    TX1REG = txData;    // Write the data byte to the USART.
}

void UARTBlockingWrite(char *data, int len)
{
    while(len--) {
        while(0 == PIR3bits.TX1IF)
        {
        }
        TX1REG = *(data++);    // Write the data byte to the USART.
    }
}


void UARTWrite(char *data, int len) {
    
    if (uart_index + to_transmit + len < UART_BUF) {
        memcpy(&uart_buffer[uart_index+to_transmit], data, len);
    }
    else {
        /* TODO: check for overflow */
        memcpy((unsigned char*)&uart_buffer[uart_index], (unsigned char*)data, UART_BUF-(uart_index+to_transmit));
        memcpy((unsigned char*)&uart_buffer[0],(unsigned char*) data, len-(UART_BUF-(uart_index+to_transmit)));
    }
    if (0 == PIR3bits.TX1IF) {
        UARTputc(uart_buffer[uart_index++]);
        to_transmit = len-1;
    }
    else {
        to_transmit += len;
    }

    PIE3bits.TX1IE = 1;
}