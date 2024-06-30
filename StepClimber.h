#pragma once
#include "ev3api.h"
#include "Motor.h"
#include "Sensor.h"
#include "Clock.h"
#include "Balancer.h"

using namespace ev3api;

class StepClimber {
private:
    Motor* leftMotor;
    Motor* rightMotor;
    GyroSensor* gyroSensor;
    Clock* clock;
    Balancer& balancer;

    void climbUp();
    void climbDown();

public:
    StepClimber(Motor* leftMotor, Motor* rightMotor, GyroSensor* gyroSensor, Clock* clock, Balancer& balancer);
    void climbStep();
};
