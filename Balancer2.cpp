#include "Balancer.h"
#include "balancer.h" // 기존의 balance_control 함수가 정의된 헤더

Balancer::Balancer(Motor* left, Motor* right, GyroSensor* gyro)
    : leftMotor(left), rightMotor(right), gyroSensor(gyro), gyroOffset(0) {}

void Balancer::initialize() {
    gyroOffset = gyroSensor->getAnglerVelocity();
    balance_init(); // 기존의 balance_init 함수 호출
}

void Balancer::control(float forward, float turn) {
    int gyro = gyroSensor->getAnglerVelocity();
    int leftEnc = leftMotor->getCounts();
    int rightEnc = rightMotor->getCounts();
    int battery = ev3_battery_voltage_mV(); // 배터리 전압을 얻는 방법이 필요합니다

    signed char pwm_L, pwm_R;
    balance_control(
        (float)forward,
        (float)turn,
        (float)gyro,
        (float)gyroOffset,
        (float)leftEnc,
        (float)rightEnc,
        (float)battery,
        &pwm_L,
        &pwm_R);

    leftMotor->setPWM(pwm_L);
    rightMotor->setPWM(pwm_R);
}