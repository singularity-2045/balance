#ifndef BALANCER_H
#define BALANCER_H

#include "ev3api.h"

class Balancer {
private:
    int gyroOffset;
    int32_t leftEncoderInitial;
    int32_t rightEncoderInitial;
    motor_port_t leftMotor;
    motor_port_t rightMotor;

public:
    Balancer(motor_port_t left, motor_port_t right);
    void initialize();
    void control(float forward, float turn);
};

#endif // BALANCER_H