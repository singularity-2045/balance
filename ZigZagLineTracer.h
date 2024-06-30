#pragma once
#include "ev3api.h"
#include "Motor.h"
#include "Sensor.h"
#include "Clock.h"
#include "Balancer.h"

using namespace ev3api;

class ZigZagLineTracer {
private:
    Balancer& balancer;
    ColorSensor* colorSensor;
    Clock* clock;

public:
    ZigZagLineTracer(Balancer& b, ColorSensor* color, Clock* c);
    void traceLine(int speed);
};
