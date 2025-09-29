#include "adc.h"

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
