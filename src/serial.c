#include "stm32l1xx.h"
#include "serial.h"

void serialInit(void)
{
	/* USART configuration structure for USART1 */
	USART_InitTypeDef usart2_init_struct;
	/* Bit configuration structure for GPIOA PIN9 and PIN10 */
	GPIO_InitTypeDef gpioa_init_struct;

	NVIC_InitTypeDef nvic_init_struct;

	/* Enalbe clock for USART1, AFIO and GPIOA */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	/* GPIOA PIN9 alternative function Tx */
	gpioa_init_struct.GPIO_Pin = GPIO_Pin_2;
	gpioa_init_struct.GPIO_Speed = GPIO_Speed_10MHz;
	gpioa_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpioa_init_struct.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &gpioa_init_struct);

	/* GPIOA PIN9 alternative function Rx */
	gpioa_init_struct.GPIO_Pin = GPIO_Pin_3;
	gpioa_init_struct.GPIO_Speed = GPIO_Speed_10MHz;
	gpioa_init_struct.GPIO_Mode = GPIO_Mode_IN;
	gpioa_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &gpioa_init_struct);

	/* Baud rate 9600, 8-bit data, One stop bit
	 * No parity, Do both Rx and Tx, No HW flow control
	 */
	usart2_init_struct.USART_BaudRate = 9600;
	usart2_init_struct.USART_WordLength = USART_WordLength_8b;
	usart2_init_struct.USART_StopBits = USART_StopBits_1;
	usart2_init_struct.USART_Parity = USART_Parity_No;
	usart2_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart2_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	/* Configure USART1 */
	USART_Init(USART2, &usart2_init_struct);
	/* Enable USART1 */
	USART_Cmd(USART2, ENABLE);

	/* Enable USART1 global interrupt */
	nvic_init_struct.NVIC_IRQChannel = USART2_IRQn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 10;
	nvic_init_struct.NVIC_IRQChannelSubPriority = 10;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&nvic_init_struct);

	/* Enable RXNE interrupt */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

uint8_t serialAvailable(void)
{

}

uint8_t serialRead(void)
{

}

void serialWrite(uint8_t data)
{

}

static void led_toggle(void)
{
	GPIO_ToggleBits(GPIOB, GPIO_Pin_6);
}

void USART2_IRQHandler(void)
{
	/* RXNE handler */
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		/* If received 't', toggle LED and transmit 'T' */
		if((char)USART_ReceiveData(USART2) == 't')
		{
			led_toggle();
			USART_SendData(USART2, 'T');

			while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
		}
	}
}
