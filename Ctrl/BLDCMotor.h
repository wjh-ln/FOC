#ifndef BLDCMOTOR_H
#define BLDCMOTOR_H

#include "foc_utils.h"

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
    float voltage_limit;
    float voltage_power_supply;
    DQVoltage_s voltage;
    void move(float new_target);
    void velocityOpenloop(float target_velocity);
    void setPhaseVoltage(float Uq, float Ud, float angle_el);
    void setPWM(float Ua, float Ub, float Uc);
    void driverInit(void);
};

#endif