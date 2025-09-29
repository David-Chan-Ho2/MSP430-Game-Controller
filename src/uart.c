#include "uart.h"

void initUART() {
  UART_SEL_REG |= RXD_PIN | TXD_PIN;
  UART_SEL2_REG |= RXD_PIN | TXD_PIN;
  UCA0CTL1 |= UCSSEL_2; // SMCLK
  UCA0BR0 = UART_BR0_9600;
  UCA0BR1 = UART_BR1_9600;
  UCA0MCTL = UCBRS0;
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;
}

static inline void uart_transmit(uint8_t data) {
  while (!(IFG2 & UCA0TXIFG))
    ;
  UCA0TXBUF = data;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
  while (!(IFG2 & UCA0TXIFG))
    ;                    // USCI_A0 TX buffer ready?
  UCA0TXBUF = UCA0RXBUF; // TX -> RXed character
}
