#include "console_uart.h"
#include "string.h"
#include "stdio.h"
#include "stdarg.h"

Console_t::Console_t(UART_HandleTypeDef *huart)
{
	Console_t::huart_m = huart;
}

void Console_t::Send(const char *str, ...)
{
    char send_buff[128];
    uint8_t buff_count = 0;
    va_list ap;
    va_start(ap, str);
    while ('\0' != *str)
    {
        switch (*str)
        {
        case '%':
            str++;
            switch (*str)
            {
                case 'd':
                    str++;
                    int val = va_arg(ap, int);
                    if (val < 0)
                    {
                        send_buff[buff_count] = '-';
                        buff_count++;
                        val = -val;
                    }
                    int val_cp = val;
                    uint8_t count = 0;
                    while (val_cp)
                    {
                        count++;
                        val_cp /= 10;
                    }
                    while (count)
                    {
                        char ch = val / _pow(10, count - 1);
                        val %= _pow(10, count - 1);
                        send_buff[buff_count] = ch + '0';
                        buff_count++;
                        count--;
                    }
                    break;
            }
            break;
        default:
            send_buff[buff_count] = *str;
            str++;
            buff_count++;
        }
    }
    HAL_UART_Transmit(huart_m, (uint8_t *)send_buff, buff_count,255);
}

void Console_t::SendInt(int num)
{	    
	char str[255];
    sprintf(str, "%d,\n", num);
	HAL_UART_Transmit_IT(huart_m,(uint8_t*)str,strlen(str));
}

void Console_t::SendFloat(float num)
{	    
	char str[255];
    sprintf(str, "%f\n", num);
	HAL_UART_Transmit_IT(huart_m,(uint8_t*)str,strlen(str));
}

void Console_t::SendThreeFloat(float num1,float num2,float num3)
{	    
	char str[255];
    sprintf(str, "%f,%f,%f\n", num1,num2,num3);
	HAL_UART_Transmit_IT(huart_m,(uint8_t*)str,strlen(str));
}

void Console_t::Open(void)
{
		HAL_UARTEx_ReceiveToIdle_IT(huart_m,rev_buff,256); 
		HAL_UART_Transmit_IT(huart_m,(uint8_t *)"Console is open!\n",sizeof("Console is open!\n"));
}

void Console_t::Receive(uint16_t size)
{
	
}

uint16_t _pow(int x, int y)
{
    uint16_t sum = 1;
    while (y--)
    {
        sum *= x;
    }
    return sum;
}