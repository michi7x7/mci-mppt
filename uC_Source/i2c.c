#include "defines.h"
#include "i2c.h"

#include "usiTwiMaster.h"
#include <avr/io.h>
#include <util/delay.h>

//returns val on success, returns negative numbers on failure
static int8_t handleSucc(uint8_t succ, int8_t val) {
	if (succ)
		return val;
	
	USI_TWI_Master_Initialise();
	
	return (-1) - USI_TWI_Get_State_Info();
}


int8_t i2c_init() {
	USI_TWI_Master_Initialise();
	return (-1) - USI_TWI_Get_State_Info();
}


int8_t i2c_sendData(uint8_t addr, void* data, uint8_t len) {
	uint8_t buffer[BUF_LEN];
	uint8_t *b, succ;
	
	if(len >= BUF_LEN)
		len = BUF_LEN-1;
	
	//set address
	buffer[0] = (addr << 1) | 0; //write
	
	//copy data
	uint8_t l = len;
	for(b = buffer+1; l > 0; --l)
		*(b++) = *((uint8_t*)(data++));
	
	succ = USI_TWI_Start_Transceiver_With_Data(buffer, len+1);
	
	return handleSucc(succ, len);
}


int8_t i2c_readData(uint8_t addr, void* data, uint8_t len) {
	uint8_t buffer[BUF_LEN];
	uint8_t *b, succ;
	
	if(len >= BUF_LEN)
		len = BUF_LEN-1;
	
	//set address
	buffer[0] = (addr << 1) | 1; //readuint16_t max_readCurr(uint8_t addr);
	
	//copy data
	uint8_t l = len;
	for(b = buffer+1; l > 1; --l)
		*(b++) = 0; //*(data++);
	
	succ = USI_TWI_Start_Transceiver_With_Data(buffer, len);
	
	if(succ) {
		//copy data
		for(b = buffer; len > 0; --len)
			*(uint8_t*)(data++) = *(b++);
	}
	
	return handleSucc(succ, len);
}


uint16_t max_readTemp(uint8_t addr) {
	static uint8_t set_ctrl_reg[] = {MAX_CTRL_REG, MAX_CTRL_TEMP};
	static uint8_t set_read_reg[] = { MAX_READ_TEMP };
	uint16_t temp;
	int8_t ret;
	
	if( (ret = i2c_sendData(addr, set_ctrl_reg, 2)) < 0)
		return 0;
	
	_delay_us(200); //delay for MAX to finish ADC
	
	if( (ret = i2c_sendData(addr, set_read_reg, 1)) < 0)
		return 0;
	
	if( (ret = i2c_readData(addr, &temp, 2)) < 0)
		return 0;
	
	return temp;
}

uint16_t max_readCurr(uint8_t addr) {
	static uint8_t set_ctrl_reg[] = {MAX_CTRL_REG, MAX_CTRL_CURR};
	static uint8_t set_read_reg[] = { MAX_READ_CURR };
	uint16_t temp;
	int8_t ret;
	
	if( (ret = i2c_sendData(addr, set_ctrl_reg, 2)) < 0)
		return -1;
	
	_delay_us(200); //delay for MAX to finish ADC
	
	if( (ret = i2c_sendData(addr, set_read_reg, 1)) < 0)
		return -1;
	
	if( (ret = i2c_readData(addr, &temp, 2)) < 0)
		return -1;
	
	return temp;
}
