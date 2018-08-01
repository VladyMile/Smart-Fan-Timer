#include "leds.h"
#include "commons.h"


//-----------------------------------------
//   Function name : LEDs_Init
//   Returns :       нет
//   Parameters :    нет
//   Purpose :       инициализация портов вывода LED
//-----------------------------------------

void LEDs_Init() {
	
	SetBit(SysLED_DDR, SysLED);	// линию SysLED настроить как вывод
	SysLED_Off();				// SysLED вЫключить
	return;
}



//-----------------------------------------
//	Function name :	SysLED_Blink
//	Returns :		нет
//	Parameters :	нет
//	Purpose :		Автомат SysLED
//					программное мигание SysLED'ом с целью
//					визуального контроля работы ядра
//-----------------------------------------

void SysLED_Blink() {

static volatile u08 state = 0;

	switch(state) 	{
	
		case 0:
			SysLED_On();			// ВКЛючаем SysLED
			GTimer_Start(timer_led_sys,SysLED_duration_ON);		// запускаем таймер
			state = 1;				// меняем состояние свича
			return;

		case 1:
			SysLED_Off();			// ВЫКЛючаем SysLED
			GTimer_Start(timer_led_sys,SysLED_duration_OFF);	// запускаем таймер
			state = 0;				// меняем состояние свича
			return;
	}
}



