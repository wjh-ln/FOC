#ifndef BLDCMOTOR_H
#define BLDCMOTOR_H

#include "foc_utils.h"
#include "as5600.h"

enum MotionControlType : uint8_t
{
    torque = 0x00,
    velocity = 0x01,
    angle = 0x02,
    velocity_openloop = 0x03,
};

typedef enum
{
    UNKNOWN = 0,  //not yet known or invalid state
    CW      = 1,  //clockwise
    CCW     = -1 // counter clockwise
       
} Direction;

class BLDCMotor
{
private:
    float shaft_angle;
    float Ua, Ub, Uc;
    float Ualpha, Ubeta;
    int dc_a;
    int dc_b;
    int dc_c;
    Direction sensor_direction;
    float zero_electric_angle;
    AS5600 sensor;
    void alignSensor(void);
public:
    uint8_t pole_pairs;
    float voltage_limit;
    float voltage_power_supply;
    uint32_t open_loop_times_pre;
    DQVoltage_s voltage;
    MotionControlType controller;
    void linkSensor(AS5600 _sensor);
    void initFOC(void);
    void move(float new_target);
    void velocityOpenloop(float target_velocity);
    void setPhaseVoltage(float Uq, float Ud, float angle_el);
    void setPWM(float Ua, float Ub, float Uc);
    void driverInit(void);
    void loopFOC(void);
};

#endif