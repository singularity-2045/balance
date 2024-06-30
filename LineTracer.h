#pragma once
#include "ev3api.h"
#include "Motor.h"
#include "Sensor.h"
#include "Clock.h"
#include "Balancer.h"

using namespace ev3api;

class LineTracer {
private:
    Motor* leftMotor;
    Motor* rightMotor;
    ColorSensor* colorSensor;
    Clock* clock;
    Balancer& balancer;

    float kp;
    float ki;
    float kd;
    float targetBrightness;
    float integral;
    float lastError;

public:
    LineTracer(Motor* leftMotor, Motor* rightMotor, ColorSensor* colorSensor, Clock* clock, Balancer& balancer, float kp, float ki, float kd, float targetBrightness);
    void traceLine(float forward);
};
