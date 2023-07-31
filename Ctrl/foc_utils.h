#ifndef FOC_UTILS_H
#define FOC_UTILS_H
#include "stdint.h"

#define COUNT_PERIOD 0XFFFF

#define _3PI_2 4.71238898038f
#define _2PI 6.28318530718f
#define _PI_2 1.57079632679f
#define _PI_3 1.0471975512f

#define _SQRT3 1.73205080757f


#define _constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

struct DQVoltage_s
{
    float d;
    float q;
};

void microsInit(void);
uint32_t getMicros(void);
float _normalizeAngle(float angle);
float _electricalAngle(float shaft_angle, int pole_pairs);

#endif