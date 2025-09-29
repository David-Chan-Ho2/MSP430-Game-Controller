#include "button.h"

volatile bool buttonPressed = false;

void initButtonGPIO() {
  // Button input with pull-up
  BUTTON_DIR_REG &= ~BUTTON_PIN;
  BUTTON_REN_REG |= BUTTON_PIN;
  BUTTON_OUT_REG |= BUTTON_PIN;

  // Button interrupt on high-to-low edge
  BUTTON_IES_REG |= BUTTON_PIN;
  BUTTON_IE_REG |= BUTTON_PIN;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port1_ISR() {
  __delay_cycles(BUTTON_DELAY);

  if ((BUTTON_IN_REG & BUTTON_PIN) == 0) {
    buttonPressed = true;
  }

  BUTTON_IFG_REG &= ~BUTTON_PIN;
}
