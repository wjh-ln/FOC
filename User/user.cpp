#include "user.h"
#include "common.h"
#include "tim.h"

LED_t LED(LED_GPIO_Port,LED_Pin,LOW);
Console_t Console(&huart1);
BLDCMotor Motor;

void Main(void)
{
    Console.Open();
    Motor.driverInit();
    while (1)
    {
        Motor.move(50);
        LED.Toggle();
    }
}