#include "PetBottleMission.h"

PetBottleMission::PetBottleMission(Balancer* b, SonarSensor* sonar, RotationCalculator* rotCalc, 
                                   Tail* t, Clock* c)
    : balancer(b), 
      sonarSensor(sonar), 
      rotationCalculator(rotCalc), 
      tail(t), 
      clock(c) {}

void PetBottleMission::searchBottle(int rotationAngle) {

    float startAngle = 0.0;
    float endAngle = 0.0;
    bool bottleDetected = false;

    rotationCalculator->reset();
    while (true) {
        balancer.control(0, 20);  // 제자리에서 회전
        tail->control(85); // 꼬리 각도를 유지

        rotationCalculator->update();
        float currentAngle = rotationCalculator->getRotationAngle();
        
        if (sonarSensor->getDistance() < 30) { // 30cm 이내로 페트병 탐지
            if (!bottleDetected) {
                startAngle = currentAngle;  // 탐지 시작 각도
                bottleDetected = true;
            }
            endAngle = currentAngle;  // 탐지 종료 각도
        }

        if (currentAngle >= rotationAngle || currentAngle <= -rotationAngle) {
            break; // 설정된 각도만큼 회전하면 탐색 중지
        }
        
        clock->sleep(4);  // 4ms 대기 (밸런서의 주기와 맞춤)
    }
    balancer.control(0, 0);  // 정지

    if (bottleDetected) {
        adjustToCenter(startAngle, endAngle);  // 페트병의 중앙으로 조정
    }
}

void PetBottleMission::pushBottleOut() {
    // 페트병 밀어내기
    for (int i = 0; i < 250; i++) { // 약 1초 동안 전진 (4ms * 250 = 1000ms)
        balancer->control(30, 0);  // 전진
        tail->control(85); // 꼬리 각도를 유지
        clock->sleep(4);  // 4ms 대기
    }
    balancer->control(0, 0);  // 정지
}

void PetBottleMission::adjustToCenter(float startAngle, float endAngle) {
    float centerAngle = (startAngle + endAngle) / 2.0;
    rotationCalculator->reset();
    
    if (centerAngle > 0) {
        // 시계 방향으로 회전
        while (rotationCalculator->getRotationAngle() < centerAngle) {
            balancer.control(0, 20);  // 제자리에서 회전
            tail->control(85); // 꼬리 각도를 유지
            rotationCalculator->update();
            clock->sleep(4);  // 4ms 대기
        }
    } else {
        // 반시계 방향으로 회전
        while (rotationCalculator->getRotationAngle() > centerAngle) {
            balancer.control(0, -20);  // 제자리에서 회전
            tail->control(85); // 꼬리 각도를 유지
            rotationCalculator->update();
            clock->sleep(4);  // 4ms 대기
        }
    }
    balancer.control(0, 0);  // 정지
}