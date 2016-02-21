#include "defines.h"
#include "pwm.h"

#include <avr/io.h>
#include <util/delay.h>

void pwm_init() {
	DDRB |= PORTB_PWM2 | PORTB_PWM1;

	// enable and lock PLL
	PLLCSR |= (1<<PLLE) | (1<<PLOCK);

	// wait for PLL to lock
	_delay_ms(10);

	// set pwm source to PLL
	PLLCSR |= (1<<PCKE);
	
	//set prescaler
	TCCR1B |= (1<<CS10); //|(1<<CS10)|(0<<CS12)|(0<CS13);
	
	// enable both PWM-Outputs
#ifdef PWM_EN_PWM1
	TCCR1A |= (1<<PWM1A)|(1<<COM1A1) | (1<<COM1A0); // PWM1
#endif

	TCCR1A |= (1<<PWM1B)|(1<<COM1B1) | (1<<COM1B0); // PWM2

	//maximum value - also defines frequency
	OCR1C = 255;

	// Compare Value
	pwm_setLvl1(0);
	pwm_setLvl2(0);
}

void pwm_setLvl1(uint8_t lvl) {
	OCR1A =	lvl;
}

void pwm_setLvl2(uint8_t lvl) {
	OCR1B = lvl;
}

uint8_t pwm_getLvl1() {
	return OCR1A;
}

uint8_t pwm_getLvl2() {
	return OCR1B;
}