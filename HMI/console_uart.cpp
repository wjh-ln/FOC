#include "console_uart.h"
#include "string.h"

Console_t::Console_t(UART_HandleTypeDef *huart)
{
	Console_t::huart_m = huart;
}

void Console_t::Send(const char * message)
{
	HAL_UART_Transmit_IT(huart_m,(uint8_t*)message,strlen(message));
}

void Console_t::Open(void)
{
		HAL_UARTEx_ReceiveToIdle_IT(huart_m,rev_buff,256); 
		HAL_UART_Transmit_IT(huart_m,(uint8_t *)"Console is open!\n",sizeof("Console is open!\n"));
}

void Console_t::Receive(uint16_t size)
{
	
}