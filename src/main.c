#include "intrinsics.h"
#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>

#include "adc.h"
#include "button.h"
#include "main.h"
#include "uart.h"

extern volatile bool buttonPressed;

int main(void) {
  init();

  while (1) {

    if (buttonPressed) {
      buttonPressed = false;
      uart_transmit(FIRE_KEY);
    }

    uint8_t value = readADC();

    if (value == FIRE_KEY) {
      value = FIRE_KEY - 1;
    }

    uart_transmit(value);
  }
}

void init() {
  BCSCTL1 = CALBC1_1MHZ; // Set DCO
  DCOCTL = CALDCO_1MHZ;

  initButtonGPIO();
  initUART();
  initADC();

  __enable_interrupt();
}
