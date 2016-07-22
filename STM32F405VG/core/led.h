#ifndef	__LED_H
#define	__LED_H

#include "stm32f4xx.h"
#include "gpio.h"

#define BYTETOBINARYPATTERN "%d%d%d%d%d%d%d%d"
#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0), \
  (byte & 0x40 ? 1 : 0), \
  (byte & 0x20 ? 1 : 0), \
  (byte & 0x10 ? 1 : 0), \
  (byte & 0x08 ? 1 : 0), \
  (byte & 0x04 ? 1 : 0), \
  (byte & 0x02 ? 1 : 0), \
  (byte & 0x01 ? 1 : 0) 

#define LED_1_GPIO PC4

#define LED_COUNT 1
typedef enum {
	LED_D1 = 1 << 0,	// 1 (00001)
} LED;
	
typedef enum {
	LED_OFF = Bit_RESET,
	LED_ON	= Bit_SET
} LED_STATE;

void led_init(void);

/** Control LEDs.
	@param led: LED(s) to be controlled.
	@param LED_STATE: LED_ON/LED_OFF
*/
void led_control(LED led, LED_STATE state);

/** Toggle LEDs.
	@param led: LED(s) to be controlled.
*/
void led_blink(LED led);

#endif /* __LED_H */
