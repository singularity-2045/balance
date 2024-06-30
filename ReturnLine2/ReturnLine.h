#pragma once
#include "ev3api.h"
#include "Motor.h"
#include "ColorSensor.h"
#include "Balancer.h"
#include "Clock.h"
#include "RotationCalculator.h"
#include "DistanceCalculator.h"

using namespace ev3api;

/* 라인트레이스의 에지 정의 */
#define R_EDGE 0 // 오른쪽 에지
#define L_EDGE 1 // 왼쪽 에지

/* 컬러 센서의 반사광으로 흰색과 검은색을 구분하는 기준 값 */
#define WHITE 40 // 흰색 기준 값
#define BLACK 10 // 검은색 기준 값

#define TASK_INTERVAL 4.0 // 태스크 주기

/* 라인 복귀 상태 전이 */
typedef enum {
    SERCH_LINE_INIT,
    SERCH_LINE,
    SET_DIRECTION_INIT,
    SET_DIRECTION
} ReturnLine_enum;

/* 라인 탐색 상태 전이 */
typedef enum {
    SERCH_L,
    BACK_L,
    SERCH_R,
    BACK_R
} serchLine_enum;

/* 차량 방향 조정 상태 전이 */
typedef enum {
    TURN_L,
    TURN_R
} setDirection_enum;

/* 라인 검출 상태 전이 */
typedef enum {
    DISCERN_WHITE1,
    DISCERN_BLACK,
    DISCERN_WHITE2
} discernLine_enum;

class ReturnLine {
private:
    Balancer* balancer;
    ColorSensor* colorSensor;
    RotationCalculator* rotationCalculator;
    DistanceCalculator* distanceCalculator;
    Clock* clock;

    ReturnLine_enum state;
    serchLine_enum serchState;
    setDirection_enum directionState;
    discernLine_enum discernState;

    int serchLineCount;
    float setDirectionLdir;
    float setDirectionRdir;
    float setDirectionPreDir;

    bool searchLine();
    bool setDirection(bool_t edge);
    bool discernLine();

    void initDirection();
    void initDiscernLine();

public:
    ReturnLine(Balancer* b, ColorSensor* color, RotationCalculator* rotCalc, DistanceCalculator* distCalc, Clock* clk);
    void init();
    bool doReturnLine(bool_t edge);
};
