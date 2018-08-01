#ifndef	LEDS_H_
#define	LEDS_H_

#include <avr/io.h>
#include "commons.h"



#define  SysLED_duration_ON		100	// длительность ВКЛ SysLED в мсек
#define  SysLED_duration_OFF	900	// длительность ВЫКЛ SysLED в мсек


/******************************************************************************************
 * Порт SysLED */

#define SysLED_DDR		DDRB
#define SysLED_PORT		PORTB
#define SysLED			0
#define SysLED_On()		do { ClearBit(SysLED_PORT, SysLED); } while (0)
#define SysLED_Off()	do { SetBit(SysLED_PORT, SysLED); } while (0)

	
/******************************************************************************************
 * Объявления функций */

void LEDs_Init();
void SysLED_Blink();


#endif // LEDS_H_
