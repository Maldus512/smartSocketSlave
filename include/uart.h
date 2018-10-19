/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */

#define UART_BUF    512

extern char uart_buffer[UART_BUF];
extern int uart_index, to_transmit;

void initUART();

void UARTputc(char c);
void UARTWrite(char* data, int len);
void UARTBlockingWrite(char* data, int len);