#ifndef __LOWPASS_FILTER_H__
#define __LOWPASS_FILTER_H__

class LowPassFilter
{
private:
    float timestamp_prev;
    float Tf;
    float y_prev;

public:
    LowPassFilter(float time_constant);
    float operator()(float x);
};

#endif