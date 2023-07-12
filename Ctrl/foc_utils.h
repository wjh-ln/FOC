#ifndef FOC_UTILS_H
#define FOC_UTILS_H

#include<stdint.h>


#define _2PI 6.28318530718f
#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

struct DQVoltage_s
{
    float d;
    float q;
};


uint32_t getMicros(void);
float _normalizeAngle(float angle);
float _electricalAngle(float shaft_angle, int pole_pairs);


#endif