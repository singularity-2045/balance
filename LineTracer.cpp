#include "LineTracer.h"

LineTracer::LineTracer(Motor* leftMotor, Motor* rightMotor, ColorSensor* colorSensor, Clock* clock, Balancer& balancer, float kp, float ki, float kd, float targetBrightness)
    : leftMotor(leftMotor), rightMotor(rightMotor), colorSensor(colorSensor), clock(clock), balancer(balancer), kp(kp), ki(ki), kd(kd), targetBrightness(targetBrightness), integral(0), lastError(0) {}

void LineTracer::traceLine(float forward) {
    float currentBrightness = colorSensor->getBrightness();
    float error = targetBrightness - currentBrightness;

    integral += error;
    float derivative = error - lastError;

    float turn = kp * error + ki * integral + kd * derivative;

    balancer.control(forward, turn);

    lastError = error;
    clock->sleep(4);  // 4ms 대기
}
