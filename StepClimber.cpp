#include "StepClimber.h"

StepClimber::StepClimber(Motor* leftMotor, Motor* rightMotor, GyroSensor* gyroSensor, Clock* clock, Balancer& balancer)
    : leftMotor(leftMotor), rightMotor(rightMotor), gyroSensor(gyroSensor), clock(clock), balancer(balancer) {}

void StepClimber::climbStep() {
    // 계단을 탐지하고 오르는 과정
    climbUp();
    // 계단을 탐지하고 내리는 과정
    climbDown();
}

void StepClimber::climbUp() {
    // 자이로 센서를 초기화하고 균형을 잡는다.
    balancer.initialize();
    
    // 계단 오르기 시작
    while (true) {
        // 자이로 센서 값과 모터 엔코더 값을 이용하여 계단을 오르는 제어
        int angle = gyroSensor->getAngle();
        
        if (angle > 10) { // 자이로 센서 값이 일정 각도 이상 기울어지면 계단을 오른 것으로 간주
            break;
        }
        
        balancer.control(30, 0); // 전진하면서 균형을 유지
        clock->sleep(10); // 10ms 대기
    }
    
    // 계단을 오르면 멈춘다
    balancer.control(0, 0);
    clock->sleep(500); // 잠시 대기하여 안정성 확보
}

void StepClimber::climbDown() {
    // 자이로 센서를 초기화하고 균형을 잡는다.
    balancer.initialize();
    
    // 계단 내리기 시작
    while (true) {
        // 자이로 센서 값과 모터 엔코더 값을 이용하여 계단을 내리는 제어
        int angle = gyroSensor->getAngle();
        
        if (angle < -10) { // 자이로 센서 값이 일정 각도 이상 기울어지면 계단을 내린 것으로 간주
            break;
        }
        
        balancer.control(-30, 0); // 후진하면서 균형을 유지
        clock->sleep(10); // 10ms 대기
    }
    
    // 계단을 내리면 멈춘다
    balancer.control(0, 0);
    clock->sleep(500); // 잠시 대기하여 안정성 확보
}
