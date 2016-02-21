#ifndef DEFINES_H
#define DEFINES_H

//-- global config
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define __ATtiny26__

// disable PWM1 for ISP
// #define PWM_EN_PWM1

//-- I/O-Ports
#define PORTB_RST (1<<7)
#define PORTB_EXT2 (1<<6)
#define PORTB_XTAL2 (1<<5)
#define PORTB_XTAL1 (1<<4)
#define PORTB_PWM2 (1<<3)
#define PORTB_SCL (1<<2)
#define PORTB_PWM1 (1<<1)
#define PORTB_SDA (1)

#define PORTA_EXT3 (1<<7)
#define PORTA_BAL3 (1<<6)
#define PORTA_BAL2 (1<<5)
#define PORTA_BAL1 (1<<4)
#define PORTA_VREF (1<<3)
#define PORTA_BAT3 (1<<2)
#define PORTA_BAT2 (1<<1)
#define PORTA_BAT1 (1)

// debug I/O
#define SET_EXT2 PORTB |= PORTB_EXT2
#define RES_EXT2 PORTB &= ~PORTB_EXT2
#define TGL_EXT2 PORTB ^= PORTB_EXT2

#define SET_EXT3 PORTA |= PORTA_EXT3
#define RES_EXT3 PORTA &= ~PORTA_EXT3
#define TGL_EXT3 PORTA ^= PORTA_EXT3

//#define SET_LEDS PORTA |= PORTA_BAL1 | PORTA_BAL2| PORTA_BAL3

// ADC-Ports
#define ADC_COUNT 3

#define ADC_BAT1 0
#define ADC_BAT2 1
#define ADC_BAT3 2


/* --- I²C Config   --- */
#define SYS_CLK   16000.0  // [kHz]
// TWI STANDARD mode timing limits. SCL <= 100kHz
#define T2_TWI    ((SYS_CLK *4700) /1000000) +1 // >4,7us
#define T4_TWI    ((SYS_CLK *4000) /1000000) +1 // >4,0us


/* --- MAX current sensor --- */
#define MAX0_ADDR 0x70
#define MAX1_ADDR 0x73

#define MAX_CTRL_REG 0x0A
#define MAX_CTRL_TEMP 0x06
#define MAX_CTRL_CURR 0x00 // 0x01: 4x gain, 0x02: 8x gain

#define MAX_READ_TEMP 0x08
#define MAX_READ_CURR 0x00 // char[2]


/* --- Arduino connection --- */
#define ARDU_ADDR 0x04 //Slave Address


#define NEVER_RETURN for(;;); return 0;

#endif
