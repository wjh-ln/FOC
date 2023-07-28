#ifndef __CONSOLE_UART_H__
#define __CONSOLE_UART_H__

#include "usart.h"

class Console_t
{
	public:
		uint8_t rev_buff[256];
		UART_HandleTypeDef *huart_m;
		Console_t(UART_HandleTypeDef *huart);
		void send(const char *str, ...);
		void Open(void);
		void Receive(uint16_t size);
		void SendInt(int num);
		void SendFloat(float num);
		void SendThreeFloat(float num1,float num2,float num3);
};

uint16_t _pow(int x, int y);

#endif