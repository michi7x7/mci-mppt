#ifndef I2C_H
#define I2C_H

#define BUF_LEN 16 //maximum message length

#define I2C_NO_DATA             (-1)  // Transmission buffer is empty
#define I2C_DATA_OUT_OF_BOUND   (-2)  // Transmission buffer is outside SRAM space
#define I2C_UE_START_CON        (-3)  // Unexpected Start Condition
#define I2C_UE_STOP_CON         (-4)  // Unexpected Stop Condition
#define I2C_UE_DATA_COL         (-5)  // Unexpected Data Collision (arbitration)
#define I2C_NO_ACK_ON_DATA      (-6)  // The slave did not acknowledge  all data
#define I2C_NO_ACK_ON_ADDRESS   (-7)  // The slave did not acknowledge  the address
#define I2C_MISSING_START_CON   (-8)  // Generated Start Condition not detected on bus
#define I2C_MISSING_STOP_CON    (-9)  // Generated Stop Condition not detected on bus

#include <inttypes.h>

/***
 Example for I²C-communication:
 
 //set some register on I²C-slave
 i2c_sendData(ADDR, {CTRL_REG_XXX, VALUE}, 2);
 
 //set read register on I²C-slave
 i2c_sendData(ADDR, {READ_XXX}, 1);
 
 //read from register
 i2c_readData(ADDR, &data, 2);
***/

/* i2c_init()
 * Initialize I²C-communication and return last error, if any
 */
int8_t i2c_init();

/* i2c_sendData(addr, data, len)
 * Send a data-packet up to BUF_LEN bytes over I²C, return error code when appropriate
 * 
 * uint8 addr: slave address (without r/w-bit)
 * void* data: pointer to data to transmit
 * uint8_t len: data length
 * returns: length or negative error codes
 */
int8_t i2c_sendData(uint8_t addr, void* data, uint8_t len);

/* i2c_readData(addr, data, len)
 * Read current register from I²C-slave
 * 
 * uint8_t addr: slave address (without r/w-bit
 * void* data:  pointer to receive buffer
 * uint8_t len: data length
 * returns: length or negative error codes
 */
int8_t i2c_readData(uint8_t addr, void* data, uint8_t len);


/* max_readCurr(addr)
 * Start ADC on current-in and read register from MAX
 * returns: adc-value or 0 on error
 */
uint16_t max_readCurr(uint8_t addr);

/* max_readTemp(addr)
 * Start ADC on temperature and read register from MAX
 * returns: adc-value or 0 on error
 */
uint16_t max_readTemp(uint8_t addr);


#endif
