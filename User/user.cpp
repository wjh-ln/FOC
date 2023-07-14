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
    Motor.voltage_power_supply = 11.1;
    Motor.voltage_limit = 8;
    while (1)
    {
        Motor.move(0.5);
        LED.Toggle();
    }
}