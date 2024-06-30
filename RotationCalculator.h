#pragma once
#include "ev3api.h"
#include "Motor.h"

using namespace ev3api;

class RotationCalculator {
private:
    Motor* leftMotor;
    Motor* rightMotor;
    int32_t leftMotorCount;
    int32_t rightMotorCount;
    float wheelDiameter;
    float wheelBase;

public:
    RotationCalculator(Motor* leftMotor, Motor* rightMotor, float wheelDiameter = 56.0, float wheelBase = 160.0);
    void reset();
    float getRotationAngle();
};
