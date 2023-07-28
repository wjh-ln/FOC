#include "BLDCMotor.h"
#include "common.h"
#include "math.h"
#include "tim.h"

// Driver Initialization
// enable SimpleFOC and Start the PWM signal generation.
void BLDCMotor::driverInit(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(Drive_EN_GPIO_Port, Drive_EN_Pin, GPIO_PIN_SET);
}

// Motor running function
// Performs the appropriate control process according to the type of setup controller.
void BLDCMotor::move(float new_targat)
{
    switch (controller)
    {
    case velocity_openloop:
        velocityOpenloop(new_targat);
        break;
    case angle:

        break;
    default:
        break;
    }
}

void BLDCMotor::loopFOC(void)
{
    if (controller == velocity_openloop)
        return;
}

// Open-loop speed control of motors.
void BLDCMotor::velocityOpenloop(float target_velocity)
{
    uint32_t now_us;
    float Ts;
    now_us = __HAL_TIM_GET_COUNTER(&htim4);
    if (now_us < open_loop_times_pre)
    {
        Ts = (0XFFFF - open_loop_times_pre + now_us) * 1e-6f;
    }
    else Ts = (now_us - open_loop_times_pre) * 1e-6f;
    open_loop_times_pre = now_us;
    shaft_angle = _normalizeAngle(shaft_angle + target_velocity * Ts);

    setPhaseVoltage(voltage_limit / 3, 0, _electricalAngle(shaft_angle, pole_pairs));
}

void BLDCMotor::setPhaseVoltage(float Uq, float Ud, float angle_el)
{
    angle_el = _normalizeAngle(angle_el);

    Ualpha = -Uq * sin(angle_el);
    Ubeta = Uq * cos(angle_el);

    Ua = Ualpha + voltage_limit / 2;
    Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_limit / 2;
    Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_limit / 2;

    setPWM(Ua, Ub, Uc);
}

void BLDCMotor::setPWM(float Ua, float Ub, float Uc)
{
    dc_a = Ua / voltage_power_supply * 1000;
    dc_b = Ub / voltage_power_supply * 1000;
    dc_c = Uc / voltage_power_supply * 1000;
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, dc_a);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, dc_b);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, dc_c);
}