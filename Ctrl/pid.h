#ifndef __PID_H__
#define __PID_H__
#include "foc_utils.h"

class PIDController
{
private:
    float P;
    float I;
    float D;
    uint32_t timestamp_prev;
public:
    PIDController(float _P,float _I,float _D);
};

#endif