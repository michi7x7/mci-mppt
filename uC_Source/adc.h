#ifndef ADC_H
#define ADC_H

#include <inttypes.h>
#include "defines.h" //for ADC_COUNT

//#define ADC_VREF_INT 1
#define ADC_VREF_EXT 1
//#define ADC_VREF_VCC 1

#define ADC_MAX 1024
#define ADC_VREF 5.3
#define ADC_PRESCALE 11.0


extern volatile uint16_t adc_val[ADC_COUNT];

/* adc_init()
 * Initialize ADC, setup compare registers
 */
void adc_init();

/* adc_start()
 * Start automatic, cyclic ADC, writes values to adc_val
 */
void adc_start();

/* adc_stop
 * Stop automatic ADC
 */
void adc_stop();

/* adc_oneshot(chnl)
 * Start single ADC-conversion, return when ADC has finished
 */
uint16_t adc_oneshot(uint8_t chnl);

#endif
