#include "pid.h"



PIDController::PIDController(float _P,float _I,float _D):P(P),I(I),D(D)
{
    timestamp_prev = getMicros();
}
