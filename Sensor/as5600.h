#ifndef AS5600_H
#define AS5600_H
#include "i2c.h"

#define AS5600_RAW_ADDR 0x36
#define AS5600_ADDR (AS5600_RAW_ADDR << 1)

#define cpr 4096

class AS5600
{
private:
    float angle_prev = 0.0f; // angle used for full rotations
    float angle_vel_prev = 0.0f; // angle used for velocity
    int32_t full_rotations;
    uint32_t velocity_times_pre;
public:
    void init(void);
    uint16_t getRawCount(void);
    float getSensorAngle(void);
    float getSensorFullAngle(void);
    int32_t getSensorRotation(void);
    float getSensorVelocity(void);
};
#endif