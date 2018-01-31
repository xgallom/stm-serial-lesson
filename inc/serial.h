#ifndef SERIAL_H_
#define SERIAL_H_

/**
  ******************************************************************************
  * @file    serial.h
  * @author  xgallom
  * @version V1.0
  * @date    01-December-2013
  * @brief   Serial interface implementation for STM32
  ******************************************************************************
*/

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
 * - General
 *   + SerialN - Pins TX, RX
 *
 * - Arduino
 * 	 + Serial1 - Pins 18, 19
 * 	 + Serial2 - Pins 16, 17
 * 	 + Serial3 - Pins 14, 15
 * 	 + Serial4 - Pins 1, 0
 * 	   This is the serial used by computer
 *
 * - STM32
 *   + Serial1 - USART1, Pins A9, A10
 *   + Serial2 - USART2, Pins A2, A3
 *   + Serial3 - USART3, To be implemented
 */
typedef enum {
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
 * @brief Initialize a serial line
 * @param serial: Specifies which Serial interface to initialize @ref EnumSerial_t
 * @return None
 */
extern void serialInit(EnumSerial_t serial);

/**
 * @brief Check whetever data is available
 * @param serial: Specifies Serial interface @ref EnumSerial_t
 * @retval true if data is available
 * @retval false otherwise
 */
extern bool serialAvailable(EnumSerial_t serial);

/**
 * @brief Check how much data is available
 * @param serial: Specifies Serial interface @ref EnumSerial_t
 * @return amount of bytes available
 */
extern size_t serialBytesAvailable(EnumSerial_t serial);

/**
 * @brief Read a byte from serial, or wait until data is available
 *
 * @note This function is blocking, for non blocking input
 * 		 first check if data is available with @ref serialAvailable
 * @param serial: Specifies Serial interface to read @ref EnumSerial_t
 * @return Data read
 */
extern uint8_t serialRead(EnumSerial_t serial);

/**
 * @brief Write a byte to serial
 *
 * @param serial: Specifies Serial interface to which to write @ref EnumSerial_t
 * @param data: Data to be written
 * @return None
 */
extern void serialWrite(EnumSerial_t serial, uint8_t data);

/* --------------------------------------------------------------------------------------------------------- */
/* IRQ Handlers declarations, required by STM library implementation */

#ifdef STM32
extern void USART1_IRQHandler(void);
extern void USART2_IRQHandler(void);
extern void USART3_IRQHandler(void);
#endif

#endif
