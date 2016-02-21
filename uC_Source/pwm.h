#ifndef PWM_H
#define PWM_H

#include <inttypes.h>

/* pwm_init()
 * Initialize pwm-outputs and start timers
 */
void pwm_init();

/* pwm_setLvl1(lvl)
 * Set PWM-level for channel 1
 */
void pwm_setLvl1(uint8_t lvl);

/* pwm_setLvl2(lvl)
 * Set PWM-level for channel 2
 */
void pwm_setLvl2(uint8_t lvl);

/* pwm_getLvl1()
 * returns: current pwm-level on channel 1
 */
uint8_t pwm_getLvl1(void);

/* pwm_getLvl2()
 * returns: current pwm-level on channel 2
 */
uint8_t pwm_getLvl2(void);

#endif
