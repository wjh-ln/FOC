#include "as5600.h"
#include "foc_utils.h"

void AS5600::init(void)
{
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