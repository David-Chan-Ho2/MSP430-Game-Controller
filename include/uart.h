#ifndef UART_H
#define UART_H

#define UART_BR0_9600 104
#define UART_BR1_9600 0

#define RXD_PIN BIT1
#define TXD_PIN BIT2
#define UART_SEL_REG P1SEL
#define UART_SEL2_REG P1SEL2

void initUART();
static inline void uart_transmit(uint8_t data);

#endif