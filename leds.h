#ifndef	LEDS_H_
#define	LEDS_H_

#include <avr/io.h>
#include "commons.h"



#define  SYS_LED_DURATION_ON	100	// длительность ВКЛ SysLED в мсек
#define  SYS_LED_DURATION_OFF	900	// длительность ВЫКЛ SysLED в мсек


/******************************************************************************************
 * Порт SysLED */

#define SYS_LED_DDR		DDRB
#define SYS_LED_PORT	PORTB
#define SYS_LED			0
#define SYS_LED_ON()	( do { CLEAR_BIT(SYS_LED_PORT,SYS_LED); } while (0))
#define SYS_LED_OFF()	( do { SET_BIT(SYS_LED_PORT,SYS_LED); } while (0))

	
/******************************************************************************************
 * Объявления функций */

void LEDs_Init();
void SysLED_Blink();


#endif // LEDS_H_
