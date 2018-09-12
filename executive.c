#include "executive.h"
#include "commons.h"
#include "adc.h"
#include "buttons.h"
#include "gtimer.h"
#include "leds.h"

//-----------------------------------------
//   Function name : EXECUTIVE_Init
//   Returns :       нет
//   Parameters :    нет
//   Purpose :       инициализация портов ввода/вывода
//-----------------------------------------

void EXECUTIVE_Init() {
	
	SET_BIT(FAN_1_DDR,FAN_1);		// линию FAN_1 настроить как вывод
	FAN_1_OFF();					// FAN_1 вЫключить

	SET_BIT(FAN_2_DDR,FAN_2);		// линию FAN_2 настроить как вывод
	FAN_2_OFF();					// FAN_2 вЫключить

	return;
}


//-----------------------------------------
//   Function name : EXECUTIVE()
//   Returns :       нет
//   Parameters :    GTIMER_RUN, BUTTONS_Get_Key(), 
//   Purpose :       главный исполнительный модуль
//-----------------------------------------

void EXECUTIVE() {

u08 key = BUTTONS_Get_Key();	// получаем из буфера код нажатой кнопки

	if (GTimer_Exp(GTIMER_RUN)) {	// если таймер RUN истёк
	
		FAN_1_OFF();				// FAN_1 вЫключить
		FAN_2_OFF();				// FAN_2 вЫключить
	
		if (key == KEY_RUN)	{		// если нажата кнопка "RUN"

			// получаем установленное "Время" работы вентиляторов
			u08 lap_time = ADC_Average_Filter_Result(LAP_TIME);

			// стартуем на установленное "Время" таймер LAP_TIME
			GTimer_Start(LAP_TIME,lap_time);

			FAN_1_ON();	// FAN_1 включаем
			FAN_2_ON();	// FAN_2 включаем
			
		}
	}
	
	else {
		
		switch (key)

	}
	return;
}


