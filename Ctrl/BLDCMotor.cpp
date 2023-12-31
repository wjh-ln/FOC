#include "BLDCMotor.h"
#include "common.h"
#include "tim.h"
#include "IQmathLib.h"
#include "math.h"
// Driver Initialization
// enable SimpleFOC and Start the PWM signal generation.
void BLDCMotor::driverInit(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(Drive_EN_GPIO_Port, Drive_EN_Pin, GPIO_PIN_SET);
}

void BLDCMotor::linkSensor(AS5600 _sensor)
{
    // TODO: Uninitialized sensor connections are not considered
    sensor = _sensor;
}

void BLDCMotor::initFOC(void)
{
    alignSensor();
}

void BLDCMotor::alignSensor(void)
{
    console.send("Align sensor.\n");
    float mid_angle, end_angle;
    float angle;
    for (int16_t i = 0; i <= 500; i++)
    {
        angle = _3PI_2 + _2PI * i / 500.0;
        setPhaseVoltage(voltage_limit / 3, 0, angle);
        HAL_Delay(2);
    }
    mid_angle = sensor.getSensorFullAngle();
    console.send("mid_angle:");
    console.SendFloat(mid_angle);

    for (int16_t i = 500; i >= 0; i--)
    {
        angle = _3PI_2 + _2PI * i / 500.0;
        setPhaseVoltage(voltage_limit / 3, 0, angle);
        HAL_Delay(2);
    }
    end_angle = sensor.getSensorFullAngle();
    console.send("end_angle:");
    console.SendFloat(end_angle);

    setPhaseVoltage(0, 0, 0);
    HAL_Delay(200);

    float moved = fabs(mid_angle - end_angle);
    if (moved < 0.06)
    {
        console.send("Failed to notice movement\n");
        sensor_direction = UNKNOWN;
    }

    else if (mid_angle < end_angle)
    {
        console.send("sensor_direction==CCW\n");
        sensor_direction = CCW;
    }
    else
    {
        console.send("sensor_direction==CW\n");
        sensor_direction = CW;
    }

    console.send("pole_pairs check:\n");
    if ((fabs(moved * pole_pairs - _2PI) > 0.5) && (moved != 0)) // 0.5 is arbitrary number it can be lower or higher!
    {
        console.send("fail - estimated pole_pairs:");

        pole_pairs = _2PI / moved + 0.5; // round up to the nearest integer
        console.send("%d", pole_pairs);
    }
    else
        console.send("pole_pairs OK!\n");

    setPhaseVoltage(voltage_limit / 3, 0, _3PI_2); // calculate the zero offset angle
    HAL_Delay(700);
    zero_electric_angle = _normalizeAngle(_electricalAngle(sensor_direction * sensor.getSensorAngle(), pole_pairs));
    HAL_Delay(20);

    setPhaseVoltage(0, 0, 0);
    if (zero_electric_angle == 0)
    {
        zero_electric_angle = DEF_ZERO_ELECTIC_ANGLE;
    }

    console.send("zero_electric_angle:");
    console.SendFloat(zero_electric_angle);
    HAL_Delay(200);
}

// Motor running function
// Performs the appropriate control process according to the type of setup controller.
void BLDCMotor::move(float new_targat)
{
    switch (controller)
    {
    case velocity_openloop:
        velocityOpenLoop(new_targat);
        break;
    case velocity:
        velocityCloseLoop(new_targat);
        break;
    case angle:
        angleCloseLoop(new_targat);
    default:
        break;
    }
}

float BLDCMotor::shaftAngle(void)
{
    return sensor_direction * LPF_angle(sensor.getSensorFullAngle());
}

float BLDCMotor::shaftVelocity(void)
{
    return sensor_direction * LPF_velocity(sensor.getSensorVelocity());
    // return sensor_direction * sensor.getSensorVelocity();
}

float BLDCMotor::electricalAngle(void)
{
    return _normalizeAngle((float)(sensor_direction * pole_pairs) * sensor.getSensorAngle() - zero_electric_angle);
}


// Open-loop speed control of motors.
void BLDCMotor::velocityOpenLoop(float target_velocity)
{
    uint32_t now_us;
    float Ts;
    now_us = getMicros();
    if (now_us < open_loop_times_pre)
        Ts = (COUNT_PERIOD - open_loop_times_pre + now_us) * 1e-6f;
    else
        Ts = (now_us - open_loop_times_pre) * 1e-6f;
    open_loop_times_pre = now_us;
    shaft_angle = _normalizeAngle(shaft_angle + target_velocity * Ts);
    voltage.q = voltage_limit / 3;
    setPhaseVoltage(voltage.q, 0, _electricalAngle(shaft_angle, pole_pairs));
}

void BLDCMotor::velocityCloseLoop(float target_velocity)
{
    shaft_velocity = shaftVelocity();

    voltage.q = PID_velocity(target_velocity - shaft_velocity);
    float electrical_angle = electricalAngle();
    setPhaseVoltage(voltage.q, 0, electrical_angle);
}

void BLDCMotor::angleCloseLoop(float target_angle)
{
    shaft_angle = shaftAngle();
    float target_velocity = PID_angle(target_angle-shaft_angle);
    shaft_velocity = shaftVelocity();
    voltage.q = PID_velocity(target_velocity - shaft_velocity);

    float electrical_angle = electricalAngle();
    setPhaseVoltage(voltage.q, 0, electrical_angle);
}

void BLDCMotor::setPhaseVoltage(float Uq, float Ud, float angle_el)
{
    switch (foc_modulation)
    {
    case SinePWM:
        angle_el = _normalizeAngle(angle_el);

        Ualpha = -Uq * sin(angle_el);
        Ubeta = Uq * cos(angle_el);

        Ua = Ualpha + voltage_limit / 2;
        Ub = (sqrt(3) * Ubeta - Ualpha) / 2 + voltage_limit / 2;
        Uc = (-Ualpha - sqrt(3) * Ubeta) / 2 + voltage_limit / 2;
        break;

    case SpaceVectorPWM:
        float Uout;
        // TODO:Ud==0 not considered
        Uout = Uq / voltage_power_supply;
        angle_el = _normalizeAngle(angle_el + _PI_2);
        int sector = floor(angle_el / _PI_3) + 1;
        float T1 = _SQRT3 * sin(sector * _PI_3 - angle_el) * Uout;
        float T2 = _SQRT3 * sin(angle_el - (sector - 1.0f) * _PI_3) * Uout;
        float T0 = 1 - T1 - T2;
        float Ta, Tb, Tc;
        switch (sector)
        {
        case 1:
            Ta = T1 + T2 + T0 / 2;
            Tb = T2 + T0 / 2;
            Tc = T0 / 2;
            break;
        case 2:
            Ta = T1 + T0 / 2;
            Tb = T1 + T2 + T0 / 2;
            Tc = T0 / 2;
            break;
        case 3:
            Ta = T0 / 2;
            Tb = T1 + T2 + T0 / 2;
            Tc = T2 + T0 / 2;
            break;
        case 4:
            Ta = T0 / 2;
            Tb = T1 + T0 / 2;
            Tc = T1 + T2 + T0 / 2;
            break;
        case 5:
            Ta = T2 + T0 / 2;
            Tb = T0 / 2;
            Tc = T1 + T2 + T0 / 2;
            break;
        case 6:
            Ta = T1 + T2 + T0 / 2;
            Tb = T0 / 2;
            Tc = T1 + T0 / 2;
            break;
        default: // possible error state
            Ta = 0;
            Tb = 0;
            Tc = 0;
        }
        Ua = Ta * voltage_limit;
        Ub = Tb * voltage_limit;
        Uc = Tc * voltage_limit;
        break;
    }
    setPWM(Ua, Ub, Uc);
}

void BLDCMotor::setPWM(float Ua, float Ub, float Uc)
{
    Ua = _constrain(Ua, 0.0f, voltage_limit);
    Ub = _constrain(Ub, 0.0f, voltage_limit);
    Uc = _constrain(Uc, 0.0f, voltage_limit);
    dc_a = Ua / voltage_power_supply * 1000;
    dc_b = Ub / voltage_power_supply * 1000;
    dc_c = Uc / voltage_power_supply * 1000;
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, dc_a);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, dc_b);
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, dc_c);
}