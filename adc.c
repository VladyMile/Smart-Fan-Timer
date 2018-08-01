#include "commons.h"
#include "adc.h" 
 
  
/**************************************************************************
*   Function name : ADC_Init
*   Returns :       нет
*   Parameters :    нет
*   Purpose :       Инициализация АЦП
****************************************************************************/

void ADC_Init(void) {
	
	SetBit(ADCSRA, ADEN);		//включаем АЦП
	ClearBit(ADCSRA, ADSC);		//старт преобразования пока не включаем
	ClearBit(ADCSRA, ADATE);	//отключаем постоянное преобразование
	SetBit(ADCSRA, ADIF);		//снимаем флаг прерывания
	SetBit(ADCSRA, ADIE);		//разрешаем прерывания

	//делитель частоты "на 32" для тактирования АЦП на (F_CPU/32) 250кГц
	SetBit(ADCSRA, ADPS2);
	ClearBit(ADCSRA, ADPS1);
	SetBit(ADCSRA, ADPS0);

	ClearBit(ADMUX, REFS1);		//Источник Опорного Напряжения выставляем как
	SetBit(ADMUX, REFS0);		//"AVcc с внешним конденсатором на ножке AREF"

	SetBit(ADMUX, ADLAR);		//выравниваем результат преобразования по левому краю
	
	ADCSRB = 0;					//мы мудрые - мы чистим этот регистр на всякий случай
	
	DIDR0 = 0;					//оставляем все входы ADC только как цифровые (Note that ADC pins
								//ADC6 and ADC7 do not have digital input buffers, and therefore
								//do not require Digital Input Disable bits) 
	
		// КАНАЛ ПРЕОБРАЗОВАНИЯ ЗДЕСЬ НЕ УКАЗЫВАЕМ
		// ПРЕОБРАЗОВАНИЕ АЦП НЕ ЗАПУСКАЕМ

	ADC_Average_Filter_Init();	// инициируем БИХ-ФНЧ сглаживания значений АЦП

}



/**************************************************************************
*   Function name : ADC_Average_Filter_Init
*   Returns :       нет
*   Parameters :    вход АЦП (ADC_State) и стартовое значение фильтра
*   Purpose :       инициализация фильтра по каждому входу
****************************************************************************/

// цифровой БИХ-ФНЧ обладает практически такой же АЧХ, что и скользящее
// среднее по 2^N, только не требует много памяти
// http://chipenable.ru/index.php/programming-avr/162-prostoy-cifrovoy-filtr.html

void ADC_Average_Filter_Init(void) {

	y[LapTime] = u08MAX << ADC_K_EXPONENT;

}


/**************************************************************************
*   Function name : ADC_Average_Filter_Update
*   Returns :       нет
*   Parameters :    принимаемые параметры - номер входа АЦП и уровень по 
*					этому входу
*   Purpose :       формирует и сохраняет оцифровку по входу, усреднённую за
*					2^N отсчётов
****************************************************************************/

// цифровой БИХ-ФНЧ обладает практически такой же АЧХ, что и скользящее
// среднее по 2^N, только не требует много памяти
// http://chipenable.ru/index.php/programming-avr/162-prostoy-cifrovoy-filtr.html

void ADC_Average_Filter_Update(u08 ch) {

//	y[ch] += ADC_Value[ADC_STATE] - (y[ch] >> ADC_K_EXPONENT);	// нечитабельно, лучше развернуть

	y[ch] = y[ch] + ADC_Value[ADC_State] - (y[ch] >> ADC_K_EXPONENT);

}


/**************************************************************************
*   Function name : ADC_Average_Filter_Result
*   Returns :       возвращает усреднённое значение по входу ADC_State АЦП
*   Parameters :    состояние АЦП ADC_State
*   Purpose :       возвращает результат АЦП по входу ADC_State, усреднённый
*					за 2^ADC_K_EXPONENT отсчётов
****************************************************************************/

// цифровой БИХ-ФНЧ обладает практически такой же АЧХ, что и скользящее
// среднее по 2^N, только не требует много памяти
// http://chipenable.ru/index.php/programming-avr/162-prostoy-cifrovoy-filtr.html

u08 ADC_Average_Filter_Result(u08 ch) {

	return y[ch] >> ADC_K_EXPONENT;

}



/**************************************************************************
*   Function name : ADC_Controller
*   Returns :       нет
*   Parameters :    нет
*   Purpose :       коммутирует вход АЦП, запускает оцифровку
*					раскладывает значения АЦП по местам, заведует
*					переключением состояния АЦП на другой вход (в перспективе)
****************************************************************************/

void ADC_Controller(void) {

// если прерывание АЦП только что случилось и ещё не обработано
	if (ADC_Latch) {

		switch (ADC_State)	{
			case LapTime: {
				// значение АЦП пропустить через фильтр для
				// удаления "дрожания" движка потенциометра "Время"
				ADC_Average_Filter_Update(ADC_State);
				
				/* ............... */
				// тут мы бы сменили ADC_State на следующее состояние,
				// но у нас других состояний пока нет, потому просто выходим
				break;
			}
			default:	{				// если мы как-то случайно тут оказались, то
				ADC_State = LapTime;	// "ремонтируем" ADC_State
				break;					// и выходим
			}
		}
		ADC_Latch = 0;			// указываем, что прерывание АЦП обработано и
		return;					// выходим из функции, чтобы не делать помногу за раз
	}


// если прерывание АЦП было обработано ранее и
// если таймер периода опроса АЦП истёк
	else if (GTimer_Exp(timer_adc)) {

		GTimer_Start(timer_adc,ADC_POLLING_PERIOD);	// рестартуем таймер АЦП

		switch (ADC_State)	{
			case LapTime:	{
				ADC_SetInput(ADC_Port[ADC_State]);	// подключаем вход АЦП к соответствующей ноге и
				ADC_Start();						// запускаем однократное преобразование АЦП
				break;								// и выходим
			}
			default:		{						// если мы как-то случайно тут оказались,
				ADC_State = LapTime;				// то быстренько "ремонтируем" ADC_State и 
				break;								// выходим
			}
		}
	}
	return;
}


