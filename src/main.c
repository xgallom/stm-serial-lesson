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

int main(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitTypeDef gpioInitTd;
	gpioInitTd.GPIO_Mode = GPIO_Mode_OUT;
	gpioInitTd.GPIO_OType = GPIO_OType_PP;
	gpioInitTd.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;

	GPIO_Init(GPIOB, &gpioInitTd);

	GPIO_SetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);

	static const uint8_t lastIndex = 2;
	static const uint32_t delaySingle = 100000;

	uint8_t delays[] = { 3, 1 };
	uint8_t index = 0, isActive = 1;

	for(;;) {
		if(isActive) {
			isActive = 0;
			GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);

			for(uint32_t time = 0; time < delays[index] * delaySingle; time++) {}
		}
		else {
			isActive = 1;
			GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);

			for(uint32_t time = 0; time < delays[index] * delaySingle; time++) {}

			index++;
			if(index == lastIndex)
				index = 0;
		}
	}
}
