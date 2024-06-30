#ifndef BALANCER_H
#define BALANCER_H

#include "Motor.h"
#include "GyroSensor.h"

class Balancer {
private:
    Motor* leftMotor;
    Motor* rightMotor;
    GyroSensor* gyroSensor;
    int gyroOffset;

public:
    Balancer(Motor* left, Motor* right, GyroSensor* gyro);
    void initialize();
    void control(float forward, float turn);
};

#endif // BALANCER_H