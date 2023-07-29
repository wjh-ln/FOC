#include "pid.h"
#include "foc_utils.h"

PIDController::PIDController(float P, float I, float D, float output_limit)
    : P(P), I(I), D(D), output_limit(output_limit)
{
    timestamp_prev = getMicros();
}

float PIDController::operator()(float error)
{
    uint32_t timestamp_now;
    float Ts;
    timestamp_now = getMicros();
    if (timestamp_now < timestamp_prev)
        Ts = (COUNT_PERIOD - timestamp_prev + timestamp_now) * 1e-6f;
    else
        Ts = (timestamp_now - timestamp_prev) * 1e-6f;
    timestamp_prev = timestamp_now;

    // u(s) = (P + I/s + Ds)e(s)
    // u_p  = P *e(k)
    float proportional = P * error;
    // u_ik = u_ik_1  + I*Ts/2*(ek + ek_1)
    float integral = integral_prev + I * Ts * (error + error_prev) * 0.5f;
    integral = _constrain(integral, -output_limit, output_limit);
    integral_prev = integral;
    // u_dk = D(ek - ek_1)/Ts
    float derivative = D * (error - error_prev) / Ts;
    float output = proportional + integral + derivative;
    error_prev = error;

    output = _constrain(output, -output_limit, output_limit);

    return output;
}