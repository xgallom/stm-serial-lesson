/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"

#include "serial.h"

static void toggleLed(void)
{
	GPIO_ToggleBits(GPIOB, GPIO_Pin_6);
}

static void setup(void)
{
	serialInit(Serial1);
}

static void loop(void)
{
	if(serialAvailable(Serial1)) {
		uint8_t c = serialRead(Serial1);

		serialWrite(Serial1, c - 'a' + 'A');

		toggleLed();
	}
}

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitTypeDef init;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOB, &init);

	setup();

	for(;;)
		loop();
}
