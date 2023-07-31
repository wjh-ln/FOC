#include "as5600.h"
#include "foc_utils.h"
#include "tim.h"
#include "math.h"

void AS5600::init(void)
{
    angle_prev = getRawCount();
}

uint16_t AS5600::getRawCount(void)
{
    uint8_t raw_angle_register = 0x0C;
    HAL_I2C_Master_Transmit(&hi2c1, AS5600_ADDR, &raw_angle_register, 1, 0xff);
    uint8_t read_buff[2];
    HAL_I2C_Master_Receive(&hi2c1, AS5600_ADDR, read_buff, 2, 0xff);
    return ((uint16_t)read_buff[0] << 8) | (uint16_t)read_buff[1];
}

float AS5600::getSensorAngle(void)
{
    return (getRawCount() / (float)cpr) * _2PI;
}

float AS5600::getSensorFullAngle(void)
{
    float d_angle, val;
    val = getSensorAngle();
    d_angle = val - angle_prev;
    if (abs(d_angle) > (0.8f * _2PI))
        full_rotations += (d_angle > 0) ? -1 : 1;
    angle_prev = val;
    return (float)full_rotations * _2PI + angle_prev;
}

int32_t AS5600::getSensorRotation(void)
{
    return full_rotations;
}

float AS5600::getSensorVelocity(void)
{
    uint32_t now_us;
    float Ts;
    now_us = getMicros();
    if (now_us < velocity_times_pre)
        Ts = (COUNT_PERIOD - velocity_times_pre + now_us) * 1e-6f;
    else
        Ts = (now_us - velocity_times_pre) * 1e-6f;
    velocity_times_pre = now_us;

    float angle_new = getSensorFullAngle();

    float velocity = (angle_new - angle_vel_prev) / Ts;
    angle_vel_prev = angle_new;
    return velocity;
}