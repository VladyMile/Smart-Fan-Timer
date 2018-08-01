#ifndef GTIMER_H_
#define GTIMER_H_

#include "commons.h"


/******************************************************************************************
 * Количество используемых таймеров (максимум 255) */

 #define MAX_GTIMERS		4

 
/******************************************************************************************
 * Идентификаторы таймеров GTimerID */

 // Идентификаторы должны идти без пропусков от 0 до 255
#define timer_led_sys	0	// системный светодиод
#define timer_buttons	1	// период опроса кнопок
#define timer_run		2	// период работы вентиляторов ("RUN")
#define timer_adc		3	// период опроса АЦП "регулятора времени"

/******************************************************************************************
 * Инициализация флага SysTick */
 
volatile u08 SysTick = 0;


/******************************************************************************************
 * Объявления функций */

void GTimer_Init(void);
void SysTime_Handler(void);
void GTimer_Start(void); 
void GTimer_Stop(void);
void GTimer_Pause(void);
void GTimer_Release(void);
void GTimer_Exp(void);
void GTimer_Get_Remainder(void);

#endif	// GTIMER_H_
