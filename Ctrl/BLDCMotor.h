#ifndef BLDCMOTOR_H
#define BLDCMOTOR_H

#include "foc_utils.h"

enum MotionControlType : uint8_t
{
    torque = 0x00,
    velocity = 0x01,
    angle = 0x02,
    velocity_openloop = 0x03,
};

class BLDCMotor
{
private:
    float shaft_angle;
    float Ua, Ub, Uc;
    float Ualpha, Ubeta;
    int dc_a;
    int dc_b;
    int dc_c;
public:
    int pole_pairs;
    float voltage_limit;
    float voltage_power_supply;
    uint32_t open_loop_times_pre;
    DQVoltage_s voltage;
    MotionControlType controller;
    void move(float new_target);
    void velocityOpenloop(float target_velocity);
    void setPhaseVoltage(float Uq, float Ud, float angle_el);
    void setPWM(float Ua, float Ub, float Uc);
    void driverInit(void);
    void loopFOC(void);
};

#endif