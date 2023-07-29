#include "foc_utils.h"
#include "common.h"
#include <math.h>
#include "tim.h"

void microsInit(void)
{
    HAL_TIM_Base_Start(&htim4);
}
uint32_t getMicros(void)
{
    return __HAL_TIM_GET_COUNTER(&htim4);
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