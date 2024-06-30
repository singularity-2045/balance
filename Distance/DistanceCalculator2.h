#ifndef DISTANCE_CALCULATOR_H
#define DISTANCE_CALCULATOR_H

#include "Motor.h"

#define TIRE_DIAMETER 81.0  // 타이어 직경 (81mm)
#define PI 3.14159265358979323846

class DistanceCalculator {
private:
    Motor* leftMotor;
    Motor* rightMotor;
    int32_t prevLeftCount;
    int32_t prevRightCount;

public:
    DistanceCalculator(Motor* left, Motor* right);
    void reset();
    float getDistance();
};

#endif // DISTANCE_CALCULATOR_H