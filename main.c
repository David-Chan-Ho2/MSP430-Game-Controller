/**
    Name: David Ho
    Date: 7/8/25
    Assignment: Lab 4
    Youtube: https://youtu.be/js3-8nyaUKg
    This Program: send button and potent through uart and serial
*/
#include "intrinsics.h"
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>

#include "button.h"

#define ADC_DELAY 300
#define BUTTON_DELAY 5000

#define UART_BR0_9600 104
#define UART_BR1_9600 0

#define RXD_PIN BIT1
#define TXD_PIN BIT2
#define UART_SEL_REG P1SEL
#define UART_SEL2_REG P1SEL2

#define FIRE_KEY 255

volatile bool buttonPressed = false;

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

  initGPIO();
  initUART();
  initADC();

  __enable_interrupt();
}

void initGPIO() {
  // Button input with pull-up
  BUTTON_DIR_REG &= ~BUTTON_PIN;
  BUTTON_REN_REG |= BUTTON_PIN;
  BUTTON_OUT_REG |= BUTTON_PIN;

  // Button interrupt on high-to-low edge
  BUTTON_IES_REG |= BUTTON_PIN;
  BUTTON_IE_REG |= BUTTON_PIN;
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

// Button interrupt
#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR() {
  __delay_cycles(BUTTON_DELAY);

  if ((BUTTON_IN_REG & BUTTON_PIN) == 0) {
    buttonPressed = true;
  }

  BUTTON_IFG_REG &= ~BUTTON_PIN;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
  while (!(IFG2 & UCA0TXIFG))
    ;                    // USCI_A0 TX buffer ready?
  UCA0TXBUF = UCA0RXBUF; // TX -> RXed character
}
