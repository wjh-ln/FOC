#include "BLDCMotor.h"
#include "common.h"
#include "math.h"
#include "tim.h"

void BLDCMotor::driverInit(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(Drive_EN_GPIO_Port,Drive_EN_Pin,GPIO_PIN_SET);
}

void BLDCMotor::move(float new_targat)
{
        velocityOpenloop(new_targat);
}

void BLDCMotor::velocityOpenloop(float target_velocity)
{
    uint32_t _miros;
    _miros = getMicros();
    float Ts = _miros * 1e-6f;

    shaft_angle = _normalizeAngle(shaft_angle + target_velocity * Ts);

    setPhaseVoltage(voltage_limit/3, 0, _electricalAngle(shaft_angle, 7));
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

    dc_a = Ua/voltage_power_supply*1000;
    dc_b = Ub/voltage_power_supply*1000;
    dc_c = Uc/voltage_power_supply*1000;
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, dc_a);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, dc_b);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, dc_c);
}