#include "PetBottleMission.h"

void PetBottleMission::searchBottle(int rotationAngle) {
    rotationCalculator->reset();
    float initialAngle = 0.0;
    float lastDetectedAngle = 0.0;
    bool detected = false;

    while (true) {
        float currentAngle = rotationCalculator->getRotationAngle();
        balancer->control(0, 20);  // 제자리에서 회전

        if (sonarSensor->getDistance() <= 10) {
            if (!detected) {
                initialAngle = currentAngle;
                detected = true;
            }
            lastDetectedAngle = currentAngle;
        }

        if (currentAngle >= rotationAngle || currentAngle <= -rotationAngle) {
            break; // 설정된 각도만큼 회전하면 탐색 중지
        }
        
        clock->sleep(4);  // 4ms 대기 (밸런서의 주기와 맞춤)
    }
    balancer->control(0, 0);  // 정지

    // 중앙 각도 계산
    float centralAngle = (initialAngle + lastDetectedAngle) / 2;
    rotationCalculator->reset();

    while (true) {
        float currentAngle = rotationCalculator->getRotationAngle();
        if (centralAngle > 0) {
            balancer->control(0, 20);  // 중앙 각도로 회전
        } else {
            balancer->control(0, -20);  // 중앙 각도로 회전
        }

        if ((centralAngle > 0 && currentAngle >= centralAngle) || (centralAngle < 0 && currentAngle <= centralAngle)) {
            break; // 중앙 각도로 회전하면 중지
        }

        clock->sleep(4);  // 4ms 대기
    }
    balancer->control(0, 0);  // 정지
}

void PetBottleMission::pushBottleOut() {
    // 페트병 밀어내기
    for (int i = 0; i < 250; i++) { // 약 1초 동안 전진 (4ms * 250 = 1000ms)
        balancer->control(30, 0);  // 전진
        clock->sleep(4);  // 4ms 대기
    }
    balancer->control(0, 0);  // 정지
}

void PetBottleMission::returnToCourse() {
    // 페트병 밀어낸 후 라인으로 돌아가기
    // 후진하면서 라인을 찾기
    for (int i = 0; i < 250; i++) { // 약 1초 동안 후진 (4ms * 250 = 1000ms)
        balancer->control(-30, 0);  // 후진
        clock->sleep(4);  // 4ms 대기
    }
    balancer->control(0, 0);  // 정지
}
