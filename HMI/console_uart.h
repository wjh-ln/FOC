#ifndef __CONSOLE_UART_H__
#define __CONSOLE_UART_H__

#include "usart.h"

class Console_t
{
	public:
		uint8_t rev_buff[256];
		UART_HandleTypeDef *huart_m;
		Console_t(UART_HandleTypeDef *huart);
		void Send(const char * message);
		void Open(void);
		void Receive(uint16_t size);
};

#endif