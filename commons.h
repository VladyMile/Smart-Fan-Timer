#ifndef COMMONS_H_
#define COMMONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <limits.h>


/******************************************************************************************
 * Битовые макросы */
#define SetBit(reg, bit)		do { reg |= (1<<bit); } while(0)
#define ClearBit(reg, bit)		do { reg &= (~(1<<bit)); } while(0)
#define SwitchBit(reg, bit)		do { reg ^= (1<<bit); } while(0)
#define BitIsSet(reg, bit)		do { ((reg & (1<<bit)) != 0); } while(0)
#define BitIsClear(reg, bit)	do { ((reg & (1<<bit)) == 0); } while(0)

/******************************************************************************************
 * Переопределения типов */
typedef	uint8_t		u08;
typedef	uint16_t	u16;
typedef	uint32_t	u32;
typedef	uint64_t	u64;

typedef	int8_t		s08;
typedef	int16_t		s16;
typedef	int32_t		s32;
typedef	int64_t		s64;

/******************************************************************************************
 * Максимальные значения типов */
 
#define u08MAX	UCHAR_MAX
#define u16MAX	USHRT_MAX
#define u32MAX	UINT_MAX
#define u64MAX	ULONG_MAX

#define s08MIN	SCHAR_MIN
#define s08MAX	SCHAR_MAX
#define s16MIN	SHRT_MIN
#define s16MAX	SHRT_MAX
#define s32MIN	INT_MIN
#define s32MAX	INT_MAX
#define s64MIN	LONG_MIN
#define s64MAX	LONG_MAX


/******************************************************************************************
 * Макросы прерываний */
 
static volatile u08 saveRegister;

#define ENABLE_INTERRUPT()	do { asm volatile ("sei"); } while(0)
#define DISABLE_INTERRUPT()	do { saveRegister = SREG; asm volatile ("cli"); } while(0)
#define RESTORE_INTERRUPT()	do { SREG = saveRegister; asm volatile ("sei"); } while(0) //использовать RESTORE только после DISABLE

#define WATCHDOG_RESET()	do { asm volatile ("wdr"); } while(0)
	
/******************************************************************************************
 * Барьер памяти */

 // http://we.easyelectronics.ru/blog/Soft/2593.html
 
#define MEMORY_BARRIER() 	do { asm volatile ("" ::: "memory"); } while(0)

/******************************************************************************************
 * Такт бездействия */

#define nop()				do { asm volatile (nop); } while(0)

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
