#include "user.h"
#include "common.h"
#include "tim.h"
#include "as5600.h"


LED_t LED(LED_GPIO_Port, LED_Pin, LOW);
Console_t console(&huart1);
BLDCMotor Motor;
AS5600 encoder;

void Main(void)
{
    Motor.voltage_power_supply = 11.1;
    Motor.voltage_limit = 10;
    Motor.pole_pairs = 7;
    Motor.controller = velocity;
    Motor.PID_velocity.output_limit = 10;
    microsInit();
    console.Open();

    Motor.driverInit();
    Motor.linkSensor(encoder);
    Motor.initFOC();

    while (1)
    {
        Motor.move(10);
        console.SendFloat(encoder.getSensorVelocity());
    }
}

