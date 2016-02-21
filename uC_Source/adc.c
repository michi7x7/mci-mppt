#include "defines.h"
#include "adc.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define CHNL_MASK 0x0F;

volatile static uint8_t cur_chnl;
volatile uint16_t adc_val[ADC_COUNT] = {};

void adc_init() {
	DDRA &= ~( PORTA_BAT1 | PORTA_BAT2 | PORTA_BAT3);
	
	// REFS1	REFS0	Referenz
	// 0    	0    	AVCC
	// 0    	1   	AREF (PA3), Internal Vref turned off
	// 1    	0   	Internal Vref 2.56, AREF (PA3) not connected
	// 1    	1   	Internal Vref 2.56 with external cap at AREF (PA3)
	
	ADMUX &= ~( (1 << REFS1) | (1 << REFS0));
	
	#ifdef ADC_VREF_INT
		ADMUX |= (1 << REFS1) | (1 << REFS0);
	#elif ADC_VREF_EXT
		ADMUX |= (0 << REFS1) | (1 << REFS0);
	#elif ADC_VREF_VCC
	ADMUX |= (0 << REFS1) | (0 << REFS0);
	#endif
	
	//ADEN	"ADC Enable": Mittels ADEN wird der ADC ein und ausgeschaltet. Eine 1 an dieser Bitposition schaltet den ADC ein.
	//ADSC	"ADC Start Conversion": Wird eine 1 an diese Bitposition geschrieben, so beginnt der ADC mit der Wandlung. Das Bit bleibt auf 1, solange die Wandlung im Gange ist. Wenn die Wandlung beendet ist, wird dieses Bit von der ADC Hardware wieder auf 0 gesetzt.
	//ADIF	"ADC Interrupt Flag": Wenn eine Messung abgeschlossen ist, wird das ADIF Bit gesetzt. Ist zusätzlich noch das ADIE Bit gesetzt, so wird ein Interrupt ausgelöst und der entsprechende Interrupt Handler angesprungen.
	//ADIE	"ADC Interrupt Enable": Wird eine 1 an ADIE geschrieben, so löst der ADC nach Beendigung einer Messung einen Interrupt aus.
	//ADPSx	"ADC Prescaler": Mit dem Prescaler kann die ADC-Frequenz gewählt werden. 50kHz - 200kHz
	// 16.000.000Hz / 200.000Hz = 80 -> 111
	ADCSR = (1<<ADEN) | (0<<ADSC) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); // TODO Crosscheck with datasheet
}

//returns voltage in millivolt
uint16_t adc_val_to_volt(uint16_t adcval) {
	return (uint16_t)( 1000.0  * (double)adcval / ADC_MAX * ADC_VREF * ADC_PRESCALE);
	
}

//0-7, 14: 1.23V, 15: 0V
static void set_chnl(uint8_t chnl) {
	cur_chnl = chnl;
	
	ADMUX &= ~CHNL_MASK;
	ADMUX |= chnl;
}

static void start_int() {
	//enable interrupt, start ADC
	ADCSR |= (1<<ADIE) | (1<<ADSC);
}


void adc_start() {
	adc_stop();
	set_chnl(0);
	
	start_int();
	sei();
}

void adc_stop() {
	//disable interrupt, stop ADC
	ADCSR &= ~( (1<<ADIE) | (1<<ADSC));
}


uint16_t adc_oneshot(uint8_t chnl) {
	adc_stop();
	set_chnl(chnl);
	
	//start ADC
	ADCSR |= (1<<ADSC);
	
	//wait for conversion finished
	while(ADCSR & (1<<ADSC));
	
	return adc_val_to_volt(ADC);
}


// --interrupt
ISR(ADC_vect)
{
	if(cur_chnl < ADC_COUNT) {
		adc_val[cur_chnl] = adc_val_to_volt(ADC);
		
		set_chnl((cur_chnl + 1) % ADC_COUNT);
		start_int();
	}
}
