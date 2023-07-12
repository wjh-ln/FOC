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
    voltage.d = 0;
    voltage.q = velocityOpenloop(new_targat);
}

float BLDCMotor::velocityOpenloop(float target_velocity)
{
    uint32_t _miros;
    _miros = getMicros();
    float Ts = _miros * 1e-6f;

    shaft_angle = _normalizeAngle(shaft_angle + target_velocity * Ts);

    setPhaseVoltage(4, 0, _electricalAngle(shaft_angle, 7));
    // HAL_Delay(50);
    return _miros;
}

void BLDCMotor::setPhaseVoltage(float Uq, float Ud, float angle_el)
{
    angle_el = _normalizeAngle(angle_el);

    Ualpha = -Uq * sin(angle_el);
    Ubeta = Uq * cos(angle_el);

    Ua = Ualpha + 8 / 2;
    Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + 8 / 2;
    Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + 8 / 2;

    setPWM(Ua, Ub, Uc);

}

void BLDCMotor::setPWM(float Ua, float Ub, float Uc)
{
    // Ua = _constrain(Ua, 0.0f, 6);
    // Ub = _constrain(Ub, 0.0f, 6);
    // Uc = _constrain(Uc, 0.0f, 6);

    dc_a = Ua/11*1000;
    dc_b = Ub/11*1000;
    dc_c = Uc/11*1000;
    // Console.SendThreeFloat(dc_a, dc_b, dc_c);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, dc_a);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, dc_b);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, dc_c);
}