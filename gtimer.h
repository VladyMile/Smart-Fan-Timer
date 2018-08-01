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
 
extern volatile u08 SysTick = 0;


/******************************************************************************************
 * Объявления функций */

void GTimer_Init();
void SysTime_Handler();
void GTimer_Start(u08 GTimerID,u64 delay); 
void GTimer_Stop(u08 GTimerID);
u08 GTimer_Exp(u08 GTimerID);
void GTimer_Pause(u08 GTimerID);
void GTimer_Release(u08 GTimerID);
u64 GTimer_Get_Remainder(u08 GTimerID);

#endif	// GTIMER_H_
