#ifndef PET_BOTTLE_MISSION_H
#define PET_BOTTLE_MISSION_H

#include "Balancer.h"
#include "SonarSensor.h"
#include "RotationCalculator.h"
#include "Tail.h"
#include "Clock.h"

class PetBottleMission {
private:
    Balancer* balancer;
    SonarSensor* sonarSensor;
    RotationCalculator* rotationCalculator;
    Tail* tail;
    Clock* clock;

public:
    PetBottleMission(Balancer* b, SonarSensor* sonar, RotationCalculator* rotCalc, 
                     Tail* t, Clock* c);

    void searchBottle(int rotationAngle);
    void pushBottleOut();
};

#endif // PET_BOTTLE_MISSION_H