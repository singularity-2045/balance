#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "ev3api.h"
#include "Motor.h"
#include "Sensor.h"
#include "Clock.h"
#include "Balancer.h"
#include "RotationCalculator.h"
#include "DistanceCalculator.h"

/* ライントレースのエッジ定義 */
#define R_EDGE 0 // 左エッジ
#define L_EDGE 1 // 右エッジ

/* カラーセンサの反射光の白黒判断基準値 */
#define WHITE 40 //白値判断基準閾値
#define BLACK 10 //黒値判断基準閾値

#define TASK_INTERVAL 4.0 // タスク周期

class ReturnLine {
private:
    enum ReturnLineState { SERCH_LINE_INIT, SERCH_LINE, SET_DIRECTION_INIT, SET_DIRECTION };
    ReturnLineState state;

    enum SerchLineState { SERCH_L, BACK_L, SERCH_R, BACK_R };
    SerchLineState serchLineState;

    enum SerchLineActionState { SERCH_TURN, SERCH_MOVE };
    SerchLineActionState serchLineActionState;

    enum SetDirectionState { TURN_L, TURN_R };
    SetDirectionState setDirectionState;

    enum DiscernLineState { DISCERN_WHITE1, DISCERN_BLACK, DISCERN_WHITE2 };
    DiscernLineState discernLineState;

    Balancer& balancer;
    ColorSensor* colorSensor;
    RotationCalculator* rotationCalculator;
    DistanceCalculator* distanceCalculator;
    Clock* clock;
    bool edge;

    static constexpr float SERCH_LINE_TURN_DIR = 45.0;
    static constexpr float SERCH_LINE_MOVE_POWER = 10;
    static constexpr float SERCH_LINE_MOVE_DIS = 100.0;
    static constexpr float SET_DIRECTION_MOVE_POWER = 5;
    static constexpr float SET_DIRECTION_DIR_DIFF = 90.0;

    float setDirectionLdir;
    float setDirectionRdir;
    float setDirectionPredir;
    int serchLineCount;

    bool searchLine();
    bool setDirection();
    bool discernLine();
    void discernLineInit();
    void setDirectionInit();

public:
    ReturnLine(Balancer& balancer, ColorSensor* colorSensor, RotationCalculator* rotationCalculator, DistanceCalculator* distanceCalculator, Clock* clock);
    void init();
    bool doReturnLine(bool edge);
};
