#include "gtimer.h"
#include "commons.h"


#define TIMER_STOPPED	0	// Таймер остановлен
#define TIMER_RUNNING	1	// Таймер работает
#define TIMER_PAUSED	2	// Таймер на паузе


volatile u08 SysTick = 0;			// Инициализация флага SysTick

static volatile u32 SysTime = 0;	// системное время

#define SYS_TIME_MAXIMUM	u32MAX	// размер должен соответствовать разрядности
									// вышеуказанной переменной SysTime

static volatile u08 GTStates[GTIMER_MAX_IDs];	// текущие состояния каждого таймера (все
												// инициализируются как TIMER_STOPPED)

static volatile u32 GTDelay[GTIMER_MAX_IDs];	// массив задержек для каждого таймера
												// GTDelay[GTimerID] = SysTime + delay;
												// все инициализируются как "истёкшие"


/**************************************************************************
*	Function name :	TIMER2_systime_Init
*	Returns :		нет
*	Parameters :	нет
*	Purpose :		TIM2 - системные часы
****************************************************************************/

void TIMER2_SysTime_Init() {
	
	DISABLE_INTERRUPT();

	//ставим режим работы - прерывание по переполнению
	CLEAR_BIT(TIMSK2, OCIE2A);
	CLEAR_BIT(TIMSK2, OCIE2B);
	SET_BIT(TIMSK2, TOIE2);
	
	//ставим предделитель таймера "1024"
	CLEAR_BIT(TCCR2B, FOC2A);
	CLEAR_BIT(TCCR2B, FOC2B);
	CLEAR_BIT(TCCR2B, WGM22);
	SET_BIT(TCCR2B, CS22);
	SET_BIT(TCCR2B, CS21);
	SET_BIT(TCCR2B, CS20);
	return;
}


/**************************************************************************
*	Function name :	GTimer_Init()
*	Returns :		нет
*	Parameters :	GTIMER_MAX_IDs - Количество используемых таймеров (максимум 255)
*					(определяется в gtimer.h)
*	Purpose :		Нулевая инициализация всех рабочих таймеров
***************************************************************************/

void GTimer_Init() {

	SysTime = 0;							// обнуляем системное время

	for(u08 i=0; i<GTIMER_MAX_IDs; i++) {	// стартуем все (определённые в GTimerID)
		GTimer_Start(i, 0);					// таймеры с временем 0 (ноль) систик
	}


}



/**************************************************************************
*	Function name :	SysTime_Handler()
*	Returns :		нет
*	Parameters :	нет
*	Purpose :		Обработчик системного времени
***************************************************************************/

void SysTime_Handler() {
	SysTick = 0;			// опускаем флаг систика

	DISABLE_INTERRUPT();
		SysTime++;			// сдвигаем стрелку системных часов
	RESTORE_INTERRUPT();
		
	// если устройство не RUN (не в работе), а системное время SysTime превысило
	// половину размера своей переменной,
	if ((GTStates[GTIMER_RUN] == TIMER_STOPPED) && ((SYS_TIME_MAXIMUM/2) < SysTime)) {
		DISABLE_INTERRUPT();
		GTimer_Init();		// то обнуляем системное время и
							// реинициализируем все рабочие таймеры
		RESTORE_INTERRUPT();
	}
}


/**************************************************************************
*	Function name :	GTimer_Start(u08 GTimerID,u32 delay)
*	Returns :		нет
*	Parameters :	u08 GTimerID - идентификатор таймера (определяется в gtimer.h)
*					u32 delay - устанавливаемое время отсчёта таймера в миллисекундах
*	Purpose :		Запуск указанного таймера на указанную длительность
***************************************************************************/

void GTimer_Start(u08 GTimerID,u32 delay) {
	GTStates[GTimerID] = TIMER_RUNNING;
	DISABLE_INTERRUPT();
		GTDelay[GTimerID] = (delay/SYS_TICK_PERIOD) + SysTime;
	RESTORE_INTERRUPT();
}


/**************************************************************************
*	Function name :	GTimer_Stop(u08 GTimerID)
*	Returns :		нет
*	Parameters :	u08 GTimerID - идентификатор таймера (определяется в gtimer.h)
*	Purpose :		По смыслу - принудительное "таймер истёк".
*					Принудительная остановка указанного таймера с обнулением длительности.
***************************************************************************/

void GTimer_Stop(u08 GTimerID)	{				
	GTStates[GTimerID] = TIMER_STOPPED;
	GTDelay[GTimerID] = 0;
}


/**************************************************************************
*	Function name :	GTimer_Exp(u08 GTimerID)
*	Returns :		YES (таймер истёк) или NO (таймер ещё не истёк)
*	Parameters :	u08 GTimerID - идентификатор таймера (определяется в gtimer.h)
*	Purpose :		Проверка истечения бегущего таймера
***************************************************************************/

u08 GTimer_Exp(u08 GTimerID) {
	if (GTStates[GTimerID] == TIMER_STOPPED) {
		return YES;
	}
	else	if (SysTime < GTDelay[GTimerID]) {
				return NO;
			}
			else {
				GTStates[GTimerID] = TIMER_STOPPED;
				return YES;
			}
	return NO;
}


/**************************************************************************
*	Function name :	GTimer_Pause(u08 GTimerID)
*	Returns :		нет
*	Parameters :	u08 GTimerID - идентификатор таймера (определяется в gtimer.h)
*	Purpose :		Пауза бегущего таймера с запоминанием остатка времени
***************************************************************************/

void GTimer_Pause(u08 GTimerID) {
	if(GTStates[GTimerID] == TIMER_RUNNING) {
		GTStates[GTimerID] = TIMER_PAUSED;
		GTDelay[GTimerID] = GTDelay[GTimerID] - SysTime;
	}
}


/**************************************************************************
*	Function name :	GTimer_Release(u08 GTimerID)
*	Returns :		нет
*	Parameters :	u08 GTimerID - идентификатор таймера (определяется в gtimer.h)
*	Purpose :		Продолжение работы таймера по остатку времени
***************************************************************************/

void GTimer_Release(u08 GTimerID)  {
	if(GTStates[GTimerID] == TIMER_PAUSED) {
		GTStates[GTimerID] = TIMER_RUNNING;
		GTDelay[GTimerID] = GTDelay[GTimerID] + SysTime;
	}
}


/**************************************************************************
*	Function name :	GTimer_Get_Remainder(u08 GTimerID)
*	Returns :		Остаток времени таймера
*	Parameters :	u08 GTimerID - идентификатор таймера (определяется в gtimer.h)
*	Purpose :		Получение остатка времени таймера
***************************************************************************/

u32 GTimer_Get_Remainder(u08 GTimerID)	{
	return ((GTDelay[GTimerID] - 2) > SysTime ? (GTDelay[GTimerID] - SysTime) : 0);
	// здесь используется (GTDelay[GTimerID] - 2), чтобы в какой-то момент времени 
	// разность u32 (GTDelay[GTimerID] - SysTime) не стала отрицательной.
	// Камень - 8-разрядный AVR, все участвующие переменные - u32, обрабатываемые за
	// несколько тактов каждая операция.
	// Поскольку операции с u32 занимают много тактов, да ещё и прерываний в устройстве
	// будет много, то разность (GTDelay[GTimerID] - SysTime) имеет вероятность
	// в какой-то момент оказаться отрицательной. И что при этом вылезет - ...
	// Можно, конечно, на время вычислений запретить прерывания, но это как-то
	// не по-людски. Прерываний и так много задумано, чтобы их ещё и душить.
	// Тем более, что точность остатка времени счётчика в моём случае всегда
	// не принципиальна, а вероятность появления этой неточности - исчезающе мала.
	// Поэтому я остановился на этом варианте - появление ошибки исключил, а на
	// маловероятное разовое появление непринципиальной погрешности в 20 мсек - наплевал.
}
