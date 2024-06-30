#include "Balancer.h"
#include "balancer.h"
#include <cstdio>

Balancer::Balancer(motor_port_t left, motor_port_t right) : 
    gyroOffset(0), 
    leftEncoderInitial(0), 
    rightEncoderInitial(0),
    leftMotor(left),
    rightMotor(right) {}

void Balancer::initialize() {
    gyroOffset = ev3_gyro_sensor_get_rate(EV3_PORT_4);
    balance_init();
    ev3_motor_reset_counts(leftMotor);
    ev3_motor_reset_counts(rightMotor);
    leftEncoderInitial = ev3_motor_get_counts(leftMotor);
    rightEncoderInitial = ev3_motor_get_counts(rightMotor);
}

void Balancer::control(float forward, float turn) {
    int gyro = ev3_gyro_sensor_get_rate(EV3_PORT_4);
    int battery = ev3_battery_voltage_mV();
    int32_t leftEncoder = ev3_motor_get_counts(leftMotor) - leftEncoderInitial;
    int32_t rightEncoder = ev3_motor_get_counts(rightMotor) - rightEncoderInitial;

    signed char pwmL, pwmR;
    balance_control(
        forward,
        turn,
        static_cast<float>(gyro),
        static_cast<float>(gyroOffset),
        static_cast<float>(leftEncoder),
        static_cast<float>(rightEncoder),
        static_cast<float>(battery),
        &pwmL,
        &pwmR
    );

    ev3_motor_set_power(leftMotor, pwmL);
    ev3_motor_set_power(rightMotor, pwmR);
}