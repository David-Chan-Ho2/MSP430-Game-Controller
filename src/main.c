#include "intrinsics.h"
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>

#include "button.h"

extern volatile bool buttonPressed;

#define ADC_DELAY 300

#define UART_BR0_9600 104
#define UART_BR1_9600 0

#define RXD_PIN BIT1
#define TXD_PIN BIT2
#define UART_SEL_REG P1SEL
#define UART_SEL2_REG P1SEL2

#define FIRE_KEY 255

void init();
void initGPIO();
void initUART();
void initADC();

static inline void uart_transmit(uint8_t data);
static inline void startAdc10();
static inline bool adcIsBusy();
static inline uint8_t readADC();

int main(void) {
  init();

  while (1) {

    if (buttonPressed) {
      buttonPressed = false;
      uart_transmit(FIRE_KEY);
    }

    uint8_t value = readADC();

    // if (value == FIRE_KEY) {
    //   value = FIRE_KEY - 1;
    // }

    uart_transmit(value);
  }
}

void init() {
  WDTCTL = WDTPW | WDTHOLD;

  BCSCTL1 = CALBC1_1MHZ; // Set DCO
  DCOCTL = CALDCO_1MHZ;

  initButtonGPIO();
  initUART();
  initADC();

  __enable_interrupt();
}

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

void initADC() {
  ADC10CTL1 = INCH_0;
  ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON;
}

static inline void startAdc10() {
  ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
}

static inline bool adcIsBusy() {
  return ADC10CTL1 & ADC10BUSY; //
}

static inline uint8_t readADC() {
  startAdc10();
  __delay_cycles(ADC_DELAY);
  while (adcIsBusy())
    ;
  return (uint8_t)(ADC10MEM >> 2); // Convert 10-bit to 8-bit
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
