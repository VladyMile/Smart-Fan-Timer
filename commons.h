#ifndef COMMONS_H_
#define COMMONS_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>


/******************************************************************************************
 * Битовые макросы */
#define SetBit(reg, bit)		do { reg |= (1<<bit); } while(0)
#define ClearBit(reg, bit)		do { reg &= (~(1<<bit)); } while(0)
#define SwitchBit(reg, bit)		do { reg ^= (1<<bit); } while(0)
#define BitIsSet(reg, bit)		do { ((reg & (1<<bit)) != 0); } while(0)
#define BitIsClear(reg, bit)	do { ((reg & (1<<bit)) == 0); } while(0)

/******************************************************************************************
 * Переопределения типов */
typedef uint8_t		u08;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef   int8_t	s08;
typedef   int16_t	s16;
typedef   int32_t	s32;
typedef   int64_t	s64;

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


/******************************************************************************************
 * Максимальные значения типов */
 
#define u08MAX	 255
#define u16MAX	 65535
#define u32MAX	 4294967295
#define u64MAX	 18446744073709551615

#define s08MIN	-128
#define s08MAX	 127
#define s16MIN	-32768
#define s16MAX	 32767
#define s32MIN	-2147483648
#define s32MAX	 2147483647
#define s64MIN	-9223372036854775808
#define s64MAX	 9223372036854775807

/*--------------------------------------------------------------------------*/
#endif /* COMMONS_H_ */
