#include "lowpass_filter.h"
#include "foc_utils.h"

LowPassFilter::LowPassFilter(float time_constant)
    : Tf(time_constant)
{
    timestamp_prev = getMicros();
}

float LowPassFilter::operator()(float x)
{
    float Ts;
    uint32_t timestamp = getMicros();
    if (timestamp < timestamp_prev)
        Ts = (COUNT_PERIOD - timestamp_prev + timestamp) * 1e-6f;
    else
        Ts = (timestamp - timestamp_prev) * 1e-6f;
    timestamp_prev = timestamp;

    if (Ts > 0.3)
    {
        y_prev = x;
        return x;
    }
    float alpha = Tf / (Tf + Ts);
    float y = alpha * y_prev + (1.0f - alpha) * x;
    y_prev = y;
    return y;
}