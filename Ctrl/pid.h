#ifndef __PID_H__
#define __PID_H__
#include "foc_utils.h"
#include "default.h"

class PIDController
{
private:
    float P;
    float I;
    float D;
    uint32_t timestamp_prev;
    float integral_prev;
    float error_prev;

public:
    float output_limit;
    PIDController(float P, float I, float D, float output_limit);
    float operator()(float error);
};

#endif