#include "foc_utils.h"
#include "common.h"
#include <math.h>
#include "tim.h"

uint32_t getMicros(void)
{
    HAL_TIM_Base_Stop(&htim4);
    uint32_t count = __HAL_TIM_GET_COUNTER(&htim4);
    __HAL_TIM_SET_COUNTER(&htim4, 0);
    HAL_TIM_Base_Start(&htim4);
    return count;
}

float _normalizeAngle(float angle)
{
    float a = fmod(angle, _2PI);
    return a >= 0 ? a : (a + _2PI);
}

float _electricalAngle(float shaft_angle, int pole_pairs)
{
    return (shaft_angle * pole_pairs);
}