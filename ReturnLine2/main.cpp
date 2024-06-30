#include "ev3api.h"
#include "app.h"
#include "Motor.h"
#include "Sensor.h"
#include "Clock.h"
#include "Balancer.h"
#include "LineTracer.h"
#include "PetBottleMission.h"
#include "GrayDetector.h"
#include "StepClimber.h"
#include "RotationCalculator.h"
#include "ReturnLine.h"
#include "DistanceCalculator.h"

using namespace ev3api;

Motor* leftMotor;
Motor* rightMotor;
Motor* tailMotor;
TouchSensor* touchSensor;
SonarSensor* sonarSensor;
ColorSensor* colorSensor;
GyroSensor* gyroSensor;
Clock* clock;
PetBottleMission* petBottleMission;
GrayDetector* grayDetector;
StepClimber* stepClimber;
LineTracer* lineTracer;
RotationCalculator* rotationCalculator;
ReturnLine* returnLine;
DistanceCalculator* distanceCalculator;
Balancer* balancer;

void main_task(intptr_t unused) {
    leftMotor = new Motor(PORT_C);
    rightMotor = new Motor(PORT_B);
    tailMotor = new Motor(PORT_A);
    touchSensor = new TouchSensor(PORT_1);
    sonarSensor = new SonarSensor(PORT_2);
    colorSensor = new ColorSensor(PORT_3);
    gyroSensor = new GyroSensor(PORT_4);
    clock = new Clock();
    
    rotationCalculator = new RotationCalculator(leftMotor, rightMotor);
    distanceCalculator = new DistanceCalculator(leftMotor, rightMotor);
    balancer = new Balancer(leftMotor, rightMotor, gyroSensor);
    petBottleMission = new PetBottleMission(balancer, sonarSensor, rotationCalculator, clock);
    grayDetector = new GrayDetector(colorSensor);
    stepClimber = new StepClimber(leftMotor, rightMotor, gyroSensor, clock, balancer);
    lineTracer = new LineTracer(leftMotor, rightMotor, colorSensor, clock, balancer, 0.6, 0.3, 0.05, 50); // PID 상수와 목표 밝기는 예시값입니다.
    returnLine = new ReturnLine(balancer, colorSensor, rotationCalculator, distanceCalculator, clock);
    
    ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
    ev3_lcd_draw_string("Robocon 2011 Course", 0, 10);

    balancer->initialize();

    while (true) {
        // 라인 복귀 실행
        if (returnLine->doReturnLine(true)) {
            // 복귀 완료 시 다른 작업 수행
        } else {
            // 평상시 라인트레이스 수행
            lineTracer->traceLine(50);
        }

        // 회색 감지 시 페트병 탐색 시작
        if (grayDetector->isGrayDetected()) {
            petBottleMission->searchBottle(45); // 예: 45도 회전하여 탐색
            petBottleMission->pushBottleOut();
            petBottleMission->returnToCourse();
        }

        // 회색 감지 시 계단 오르기 시작
        if (grayDetector->isGrayDetected()) {
            stepClimber->climbStep();
        }

        clock->sleep(10); // 10 ms 대기
    }
    
    // 사용이 끝난 후 메모리 해제
    delete leftMotor;
    delete rightMotor;
    delete tailMotor;
    delete touchSensor;
    delete sonarSensor;
    delete colorSensor;
    delete gyroSensor;
    delete clock;
    delete petBottleMission;
    delete grayDetector;
    delete stepClimber;
    delete lineTracer;
    delete rotationCalculator;
    delete returnLine;
    delete distanceCalculator;
    delete balancer;
}
