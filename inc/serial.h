#ifndef SERIAL_H_
#define SERIAL_H_

#include "defines.h"

#include <stddef.h>

#ifdef ARDUINO

#define uint8_t byte
#define uint16_t word

#else

#endif

/* --------------------------------------------------------------------------------------------------------- */

/**
 * @brief List of possible Serial interfaces available on the platform
 *
 * @platform General:
 *   @arg SerialN - Pins TX, RX
 *
 * @platform Arduino:
 * 	 @arg Serial1 - Pins 18, 19
 * 	 @arg Serial2 - Pins 16, 17
 * 	 @arg Serial3 - Pins 14, 15
 * 	 @arg Serial4 - Pins 1, 0
 * 	   @note This is the serial used by computer
 *
 * @platform STM32:
 *   @arg Serial1 - USART1, Pins A9, A10
 *   @arg Serial2 - USART2, Pins A2, A3
 *   @arg Serial3 - USART3, To be implemented
 */
typedef enum EnumSerial {
	Serial1 = 0,
	Serial2,
	Serial3,
#ifdef ARDUINO
	Serial4,
#endif

	/* !important: Always keep last */
	SerialSize
} EnumSerial_t;

/* --------------------------------------------------------------------------------------------------------- */

/**
 * @brief Initialize a Serial line
 * @param serial: Specifies which Serial interface to initialize @ref EnumSerial_t
 * @retval None
 */
extern void serialInit(EnumSerial_t serial);

/**
 * @brief Check whetever data is available
 * @param serial: Specifies Serial interface @ref EnumSerial_t
 * @retval true if data is available, else false
 */
extern bool serialAvailable(EnumSerial_t serial);

/**
 * @brief Check how much data is available
 * @param serial: Specifies Serial interface @ref EnumSerial_t
 * @retval amount of bytes available
 */
extern size_t serialBytesAvailable(EnumSerial_t serial);

/**
 * @brief Read a byte from serial, or wait until data is available
 *
 * @note This function is blocking, for non blocking input
 * 		 first check if data is available with @ref serialAvailable
 * @param serial: Specifies Serial interface to read @ref EnumSerial_t
 * @retval true if data is available, else false
 */
extern uint8_t serialRead(EnumSerial_t serial);

/**
 * @brief Write a byte to serial
 *
 * @param serial: Specifies Serial interface to which to write @ref EnumSerial_t
 * @param data: Data to be written
 * @retval None
 */
extern void serialWrite(EnumSerial_t s, uint8_t data);

/* --------------------------------------------------------------------------------------------------------- */
/* IRQ Handlers declarations, required by STM library implementation */

#ifdef STM32
extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);
extern void USART3_IRQHandler(void);
#endif

#endif
