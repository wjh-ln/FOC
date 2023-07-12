#include "user.h"
#include "led.h"

LED_t LED(LED_GPIO_Port,LED_Pin,LOW);

void Main(void)
{
    while (1)
    {
        LED.Toggle();
        HAL_Delay(300);
    }
}