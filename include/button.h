#ifndef BUTTON_H
#define BUTTON_H

#include <msp430.h>
#include <stdbool.h>

#define BUTTON_DELAY 5000

#define BUTTON_PIN BIT3
#define BUTTON_DIR_REG P1DIR
#define BUTTON_REN_REG P1REN
#define BUTTON_OUT_REG P1OUT
#define BUTTON_IN_REG P1IN

#define BUTTON_IES_REG P1IES
#define BUTTON_IE_REG P1IE
#define BUTTON_IFG_REG P1IFG

void initButtonGPIO();

#endif
