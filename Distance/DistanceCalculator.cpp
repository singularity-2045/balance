#include "DistanceCalculator.h"
#include <cmath>

DistanceCalculator::DistanceCalculator(Motor* leftMotor, Motor* rightMotor)
    : leftMotor(leftMotor), rightMotor(rightMotor) {
    reset();
}

void DistanceCalculator::reset() {
    leftMotorAngle = leftMotor->getCount();
    rightMotorAngle = rightMotor->getCount();
}

float DistanceCalculator::getDistance() {
    float currentLeftAngle = leftMotor->getCount();
    float currentRightAngle = rightMotor->getCount();

    float leftDistance = (WHEEL_DIAMETER * M_PI * (currentLeftAngle - leftMotorAngle)) / 360.0;
    float rightDistance = (WHEEL_DIAMETER * M_PI * (currentRightAngle - rightMotorAngle)) / 360.0;

    // 평균 이동 거리를 계산
    float distance = (leftDistance + rightDistance) / 2.0;

    return distance;
}
