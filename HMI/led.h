#ifndef __LED_H__
#define __LED_H__

#include "gpio.h"


#ifndef EFFECT_LEVEL
#define EFFECT_LEVEL

	typedef enum
	{
		HIGH = 0u,
		LOW
	} Effective_Level;

#endif


class LED_t
{
	private:
		GPIO_TypeDef *GPIOx_m; 
		uint16_t GPIO_Pin_m;
		Effective_Level Effective_Level_m;
	
	public:
		LED_t(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,Effective_Level Effective_Level_x);
		void ON(void);
		void OFF(void);
		void Toggle(void);
};

#endif