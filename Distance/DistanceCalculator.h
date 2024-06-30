#pragma once
#include "ev3api.h"
#include "Motor.h"

#define WHEEL_DIAMETER 56.0  // 바퀴 직경 (mm)

using namespace ev3api;

class DistanceCalculator {
private:
    Motor* leftMotor;
    Motor* rightMotor;
    float leftMotorAngle;
    float rightMotorAngle;

public:
    DistanceCalculator(Motor* leftMotor, Motor* rightMotor);
    void reset();
    float getDistance();
};
