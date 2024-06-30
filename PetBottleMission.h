#pragma once
#include "ev3api.h"
#include "Motor.h"
#include "Sensor.h"
#include "Clock.h"
#include "Balancer.h"
#include "RotationCalculator.h"

using namespace ev3api;

class PetBottleMission {
private:
    Balancer* balancer;
    SonarSensor* sonarSensor;
    RotationCalculator* rotationCalculator;
    Clock* clock;

public:
    PetBottleMission(Balancer* b, SonarSensor* sonar, RotationCalculator* rotCalc, Clock* c)
        : balancer(b), sonarSensor(sonar), rotationCalculator(rotCalc), clock(c) {}

    void searchBottle(int rotationAngle);
    void pushBottleOut();
    void returnToCourse();
};
