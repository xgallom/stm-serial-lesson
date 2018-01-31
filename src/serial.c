#include "stm32l1xx.h"
#include "serial.h"

#include <string.h>

/* --------------------------------------------------------------------------------------------------------- */
/* Internal Type Definitions */

typedef struct SerialData {
	uint8_t data[BUF_SIZE];
	size_t dataCurrent, dataSize;
} SerialData_t;

/* --------------------------------------------------------------------------------------------------------- */
/* Static Variable Declarations */

static SerialData_t serialData[SerialSize];
static bool sSerialIsInit[SerialSize] = { false, false, false };
static USART_TypeDef *usarts[SerialSize] = { USART1, USART2, USART3 };

/* --------------------------------------------------------------------------------------------------------- */
/* Static Function Declarations */

static uint8_t getData(SerialData_t *ser);
static void setData(SerialData_t *ser, uint8_t data);
static SerialData_t *getSerialData(enum EnumSerial s);
static bool serialIsInit(enum EnumSerial s);

/* --------------------------------------------------------------------------------------------------------- */
/* Extern Function Implementations */

void serialInit(EnumSerial_t s)
{
	if(serialIsInit(s))
		return;

	/* STM USART Configuration */
	USART_InitTypeDef usart_init_struct;
	GPIO_InitTypeDef gpio_init_struct;
	NVIC_InitTypeDef nvic_init_struct;

	gpio_init_struct.GPIO_Speed = GPIO_Speed_10MHz;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;

	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init_struct.NVIC_IRQChannelSubPriority = 0;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;

	switch(s) {
	case Serial1:
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

		gpio_init_struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

		nvic_init_struct.NVIC_IRQChannel = USART1_IRQn;
		break;

	case Serial2:
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

		gpio_init_struct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

		nvic_init_struct.NVIC_IRQChannel = USART2_IRQn;
		break;

	case Serial3:
		return;

	default:
		return;
	}

	GPIO_Init(GPIOA, &gpio_init_struct);

	usart_init_struct.USART_BaudRate = 9600;
	usart_init_struct.USART_WordLength = USART_WordLength_8b;
	usart_init_struct.USART_StopBits = USART_StopBits_1;
	usart_init_struct.USART_Parity = USART_Parity_No;
	usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(usarts[s], &usart_init_struct);
	USART_Cmd(usarts[s], ENABLE);

	NVIC_Init(&nvic_init_struct);

	USART_ITConfig(usarts[s], USART_IT_RXNE, ENABLE);

	/* Internal Variable Modifications */
	sSerialIsInit[s] = true;
	memset(serialData[s].data, 0, BUF_SIZE);
	serialData[s].dataCurrent = serialData[s].dataSize = 0;
}

bool serialAvailable(EnumSerial_t s)
{
	return serialBytesAvailable(s) ? true : false;
}

size_t serialBytesAvailable(EnumSerial_t s)
{
	SerialData_t *ser = getSerialData(s);

	if(ser->dataSize < ser->dataCurrent)
		return BUF_SIZE + ser->dataSize - ser->dataCurrent;
	else
		return ser->dataSize - ser->dataCurrent;
}

uint8_t serialRead(EnumSerial_t s)
{
	while(!serialAvailable(s))
	{}

	return getData(getSerialData(s));
}

void serialWrite(EnumSerial_t s, uint8_t data)
{
	USART_SendData(usarts[s], data);
}

/* --------------------------------------------------------------------------------------------------------- */
/* Static Function Implementations */

static SerialData_t *getSerialData(EnumSerial_t s)
{
	return &serialData[s];
}

static bool serialIsInit(EnumSerial_t s)
{
	return sSerialIsInit[s];
}

static uint8_t getData(SerialData_t *ser)
{
	uint8_t retval = ser->data[ser->dataCurrent++];

	if(ser->dataCurrent >= BUF_SIZE)
		ser->dataCurrent = 0;

	return retval;
}

static void setData(SerialData_t *ser, uint8_t data)
{
	ser->data[ser->dataSize++] = data;

	if(ser->dataSize >= BUF_SIZE)
		ser->dataSize = 0;
}

/* --------------------------------------------------------------------------------------------------------- */
/* IRQ Handlers Implementation */

#define USART_IRQ_HANDLER_IMPL(N) \
		void USART##N##_IRQHandler(void) \
		{ \
			if(USART_GetITStatus(USART##N, USART_IT_RXNE) != RESET) \
				setData(getSerialData(Serial##N), USART_ReceiveData(USART##N)); \
		}

USART_IRQ_HANDLER_IMPL(1)
USART_IRQ_HANDLER_IMPL(2)
USART_IRQ_HANDLER_IMPL(3)

#undef USART_IRQ_HANDLER_IMPL
