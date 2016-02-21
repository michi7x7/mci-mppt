#include "defines.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <inttypes.h>

#include "i2c.h"
#include "pwm.h"
#include "adc.h"

typedef struct {
	uint16_t current1;
	uint16_t current2;
	uint16_t temp1;
	uint16_t temp2;
	uint16_t bat1;
	uint16_t bat2;
	uint16_t bat3;
	uint16_t pow1;
	uint16_t pow2;
	uint8_t pwm1;
	uint8_t pwm2;
} state_t;

enum VARS {
	VAR_BAT1 = 1,
	VAR_BAT2 = 2,
	VAR_BAT3 = 3,
	
	VAR_TMP1 = 4,
	VAR_TMP2 = 5,
	
	VAR_CUR1 = 6,
	VAR_CUR2 = 7,
	
	VAR_PWM1 = 10,
	VAR_PWM2 = 11
};

//uint8_t buf[3];

void report(uint8_t cmd, uint16_t val) {
	struct {
		uint8_t cmd;
		uint16_t val;
		} data;
	
	data.cmd = cmd;
	data.val = val;
	
	i2c_sendData(ARDU_ADDR, &data, 3); // TODO send internal state
}

void setup(void) {
	DDRA = PORTA_EXT3 | PORTA_BAL3 | PORTA_BAL2 | PORTA_BAL1;
	DDRB = PORTB_EXT2;
	
	//pullups / outputs
	PORTB = PORTB_RST;
}

int main(void) {
	state_t cur_state = {0};
	state_t old_state = {0};
	
	
	setup();
	pwm_init();  //start PWM-generators, update on pwm_setLvl-commands
	i2c_init();  //initialize I²C-communication, ready for i2c_-commands
	adc_init();  //initialize ADC-Ports
	
	adc_start(); //start cyclic ADC-conversion, will update adc_val continuously
	
	_delay_ms(1000);
	
	//SET_LEDS;
	
	for(;;) {
		TGL_EXT3;
		
		// read current
		cur_state.current1 = max_readCurr(MAX0_ADDR);
		cur_state.current2 = max_readCurr(MAX1_ADDR);
		
		// read temp
		cur_state.temp1 = max_readTemp(MAX0_ADDR);
		cur_state.temp2 = max_readTemp(MAX1_ADDR);
		TGL_EXT2;
	
		cur_state.bat1 = adc_val[ADC_BAT1]; 
		cur_state.bat2 = adc_val[ADC_BAT2]; 
		cur_state.bat3 = adc_val[ADC_BAT3];
		
		cur_state.pow1 = cur_state.bat1 * cur_state.current1;
		cur_state.pow2 = cur_state.bat1 * cur_state.current2;
		
		cur_state.pwm1 = pwm_getLvl1();
		cur_state.pwm2 = pwm_getLvl2();
		
		// balance battery cells
		// TODO
		
		// only do something if battery isn't already full
		if(cur_state.bat1 < 12.6) {
			
			// PWM1: if power has improved since last iteration -> increase pwm
			if(cur_state.pow1 > old_state.pow1) {
				cur_state.pwm2 += 1; // TODO adaptive gain
			}
			
			// PWM1: if power has decreased since last iteration -> decrease pwm
			if(cur_state.pow1 < old_state.pow1) {
				cur_state.pwm1 -= 1; // TODO adaptive gain
			}
			
			// PWM2: if power has improved since last iteration -> increase pwm
			if(cur_state.pow2 > old_state.pow2) {
				cur_state.pwm2 += 1; // TODO adaptive gain
			}
			
			// PWM2: if power has decreased since last iteration -> decrease pwm
			if(cur_state.pow2 < old_state.pow2) {
				cur_state.pwm2 -= 1; // TODO adaptive gain
			}
			
			} else {
			// turn off pwm
			cur_state.pwm1 = 0;
			cur_state.pwm2 = 0;
		}
		
		pwm_setLvl1(cur_state.pwm1);
		pwm_setLvl2(cur_state.pwm2);
		
		// report internal state w/ i2c
		report(VAR_BAT1, cur_state.bat1);
		report(VAR_BAT2, cur_state.bat2);
		report(VAR_BAT3, cur_state.bat3);
		
		PORTA ^= PORTA_BAL2;
				
		report(VAR_TMP1, cur_state.temp1);
		report(VAR_TMP2, cur_state.temp2);
				
		report(VAR_CUR1, cur_state.current1);
		report(VAR_CUR2, cur_state.current2);
		
		report(VAR_PWM1, cur_state.pwm1);
		report(VAR_PWM2, cur_state.pwm2);
		
		// copy states
		old_state = cur_state;
		
		// TODO adaptive sleep
		_delay_ms(100);
	}

	NEVER_RETURN;
}
