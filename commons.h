#ifndef COMMONS_H_
#define COMMONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <avr/wdt.h>
#include <avr/cpufunc.h>


/******************************************************************************************
* Битовые макросы */
#define SET_BIT(reg, bit)		(reg |= (1<<bit))
#define CLEAR_BIT(reg, bit)		(reg &= (~(1<<bit)))
#define SWITCH_BIT(reg, bit)	(reg ^= (1<<bit))
#define BIT_IS_SET(reg, bit)	((reg & (1<<bit)) != 0)
#define BIT_IS_CLEAR(reg, bit)	((reg & (1<<bit)) == 0)

/******************************************************************************************
* Переопределения типов */
typedef uint8_t		u08;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef int8_t		s08;
typedef int16_t		s16;
typedef int32_t		s32;
typedef int64_t		s64;

/******************************************************************************************
* Максимальные значения типов */

#define u08MAX	UINT8_MAX
#define u16MAX	UINT16_MAX
#define u32MAX	UINT32_MAX
#define u64MAX	UINT64_MAX

#define s08MIN	INT8_MIN
#define s08MAX	INT8_MAX
#define s16MIN	INT16_MIN
#define s16MAX	INT16_MAX
#define s32MIN	INT32_MIN
#define s32MAX	INT32_MAX
#define s64MIN	INT64_MIN
#define s64MAX	INT64_MAX

/******************************************************************************************
* Макросы прерываний */

static volatile u08 SaveRegister;

#define ENABLE_INTERRUPT()	sei()
#define DISABLE_INTERRUPT()	(do { SaveRegister = SREG; cli(); } while(0))
#define RESTORE_INTERRUPT()	(do { SREG = SaveRegister; sei(); } while(0))	//использовать
																			//RESTORE только
																			//после DISABLE

#define WATCHDOG_RESET()	wdt_reset()		// пинок собаке
   
/******************************************************************************************
* Барьер памяти */

// http://we.easyelectronics.ru/blog/Soft/2593.html

#define MEMORY_BARRIER()	_MemoryBarrier();

/******************************************************************************************
* Такт бездействия */

#define NOP()				_NOP();	// единица измерения - 1 лень

/******************************************************************************************
* Логические значения */

#define TRUE	1
#define FALSE	0

#define HIGH	1
#define LOW		0

#define ON		1
#define OFF		0

#define YES		1
#define NO		0


/*--------------------------------------------------------------------------*/
#endif /* COMMONS_H_ */