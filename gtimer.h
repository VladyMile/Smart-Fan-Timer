#ifndef GTIMER_H_
#define GTIMER_H_

#include "commons.h"


/******************************************************************************************
 * Количество используемых таймеров (максимум 255) */

 #define GTIMER_MAX_IDs		4

 
/******************************************************************************************
 * Идентификаторы таймеров GTimerID */

#define SYS_TICK_PERIOD         10                      // период системных часов, в миллисекундах

 // Идентификаторы должны идти без пропусков от 0 до 255
#define GTIMER_SYS_LED	0	// системный светодиод
#define GTIMER_BUTTONS	1	// период опроса кнопок
#define GTIMER_RUN		2	// период работы вентиляторов ("RUN")
#define GTIMER_ADC		3	// период опроса АЦП

/******************************************************************************************
 * Инициализация флага SysTick */
 
extern volatile u08 SysTick;


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
