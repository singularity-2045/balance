#include "DistanceCalculator.h"

DistanceCalculator::DistanceCalculator(Motor* left, Motor* right)
    : leftMotor(left), rightMotor(right) {
    reset();
}

void DistanceCalculator::reset() {
    prevLeftCount = leftMotor->getCounts();
    prevRightCount = rightMotor->getCounts();
}

float DistanceCalculator::getDistance() {
    int32_t currentLeftCount = leftMotor->getCounts();
    int32_t currentRightCount = rightMotor->getCounts();

    float leftDistance = ((PI * TIRE_DIAMETER) / 360.0) * (currentLeftCount - prevLeftCount);
    float rightDistance = ((PI * TIRE_DIAMETER) / 360.0) * (currentRightCount - prevRightCount);

    prevLeftCount = currentLeftCount;
    prevRightCount = currentRightCount;

    return (leftDistance + rightDistance) / 2.0;
}