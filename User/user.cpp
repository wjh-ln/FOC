#include "user.h"
#include "common.h"
#include "tim.h"
#include "as5600.h"
#include "IQmathLib.h"

LED_t LED(LED_GPIO_Port, LED_Pin, LOW);
Console_t console(&huart1);
BLDCMotor Motor;
AS5600 encoder;
float user_cmd;
void Main(void)
{
    Motor.voltage_power_supply = 11.1;
    Motor.voltage_limit = 10.0;
    Motor.pole_pairs = 7;
    Motor.controller = angle;
    Motor.PID_velocity.output_limit = Motor.voltage_limit;
    Motor.foc_modulation = SpaceVectorPWM;
    microsInit();
    console.Open();

    Motor.driverInit();
    Motor.linkSensor(encoder);
    Motor.initFOC();

    while (1)
    {
        Motor.move(user_cmd); 
        LED.Toggle();    
    }
}

