#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>

#define UART_BUF    64

extern char uart_write_buffer[UART_BUF];
extern int uart_tx_index, to_transmit;


typedef enum {
    NO_CMD,
    PRINT_READING,
            ON,
            OFF,
            CALIBRATE,
            RESET,
            READ_CURRENT,
            ZERO_POWER,
            READ_POWER,
            PRINT_STATE
} command_t;

void initUART();

void UARTputc(uint8_t c);
void UARTBlockingWrite(char* data, int len);
void UARTReceivedChar(char data);

command_t nextCommand();

#endif
