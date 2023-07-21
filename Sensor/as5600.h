#ifndef AS5600_H
#define AS5600_H
#include "i2c.h"


#define AS5600_RAW_ADDR    0x36
#define AS5600_ADDR        (AS5600_RAW_ADDR << 1)

#define cpr 4096

class AS5600
{
private:
    float angle_data_prev;
public:
    void init(void);
    uint16_t getRawCount(void);
    float getSensorAngle(void);
};

#endif