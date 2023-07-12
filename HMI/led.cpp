#include "led.h"

LED_t::LED_t(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,Effective_Level Effective_Level_x)
{
		LED_t::GPIOx_m = GPIOx;
		LED_t::GPIO_Pin_m = GPIO_Pin;
		LED_t::Effective_Level_m = Effective_Level_x;
}

void LED_t::ON(void)
{
		if(Effective_Level_m==HIGH)
			HAL_GPIO_WritePin(GPIOx_m,GPIO_Pin_m,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOx_m,GPIO_Pin_m,GPIO_PIN_RESET);
}

void LED_t::OFF(void)
{
		if(Effective_Level_m==LOW)
			HAL_GPIO_WritePin(GPIOx_m,GPIO_Pin_m,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOx_m,GPIO_Pin_m,GPIO_PIN_RESET);
}

void LED_t::Toggle(void)
{
		HAL_GPIO_TogglePin(GPIOx_m,GPIO_Pin_m);
}
