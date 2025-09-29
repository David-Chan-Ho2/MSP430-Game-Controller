#ifndef ADC_H
#define ADC_H

#define ADC_DELAY 300

void initADC();

static inline void startAdc10();
static inline bool adcIsBusy();
static inline uint8_t readADC();

#endif