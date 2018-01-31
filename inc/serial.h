#ifndef SERIAL_H_
#define SERIAL_H_

#include "defines.h"

#include <stddef.h>

#ifdef ARDUINO

#define uint8_t byte
#define uint16_t word

#else

#endif

enum EnumSerial {
	Serial1 = 0,
	Serial2,
	Serial3,
#ifdef ARDUINO
	Serial,
#endif

	/* Always keep last */
	SerialSize
};

extern void serialInit(enum EnumSerial);
extern bool serialAvailable(enum EnumSerial);
extern size_t serialBytesAvailable(enum EnumSerial);
extern uint8_t serialRead(enum EnumSerial);
extern void serialWrite(enum EnumSerial, uint8_t data);

#ifdef STM32
extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);
extern void USART3_IRQHandler(void);
#endif

#endif
