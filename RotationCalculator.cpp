#include "RotationCalculator.h"
#include <cmath>

RotationCalculator::RotationCalculator(Motor* leftMotor, Motor* rightMotor, float wheelDiameter, float wheelBase)
    : leftMotor(leftMotor), rightMotor(rightMotor), wheelDiameter(wheelDiameter), wheelBase(wheelBase) {
    reset();
}

void RotationCalculator::reset() {
    leftMotorCount = leftMotor->getCount();
    rightMotorCount = rightMotor->getCount();
}

float RotationCalculator::getRotationAngle() {
    int32_t currentLeftMotorCount = leftMotor->getCount();
    int32_t currentRightMotorCount = rightMotor->getCount();

    int32_t deltaLeft = currentLeftMotorCount - leftMotorCount;
    int32_t deltaRight = currentRightMotorCount - rightMotorCount;

    leftMotorCount = currentLeftMotorCount;
    rightMotorCount = currentRightMotorCount;

    float leftDistance = (deltaLeft / 360.0) * M_PI * wheelDiameter;
    float rightDistance = (deltaRight / 360.0) * M_PI * wheelDiameter;

    float angle = (leftDistance - rightDistance) / wheelBase * (180.0 / M_PI); // 각도를 도 단위로 변환

    return angle;
}
