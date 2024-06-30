#include "ReturnLine.h"

ReturnLine::ReturnLine(Balancer& balancer, ColorSensor* colorSensor, RotationCalculator* rotationCalculator, DistanceCalculator* distanceCalculator, Clock* clock)
    : balancer(balancer), colorSensor(colorSensor), rotationCalculator(rotationCalculator), distanceCalculator(distanceCalculator), clock(clock), state(SERCH_LINE_INIT), serchLineState(SERCH_L), serchLineActionState(SERCH_TURN), setDirectionState(TURN_R), edge(false), serchLineCount(1), setDirectionLdir(SET_DIRECTION_DIR_DIFF), setDirectionRdir(SET_DIRECTION_DIR_DIFF), setDirectionPredir(0.0) {}

void ReturnLine::init() {
    state = SERCH_LINE_INIT;
    serchLineState = SERCH_L;
    serchLineActionState = SERCH_TURN;
    serchLineCount = 1;
    setDirectionInit();
    discernLineInit();
}

bool ReturnLine::doReturnLine(bool edge) {
    this->edge = edge;
    switch(state) {
    case SERCH_LINE_INIT:
        init();
        state = SERCH_LINE;
        break;
    case SERCH_LINE:
        if (searchLine()) {
            state = SET_DIRECTION_INIT;
        }
        break;
    case SET_DIRECTION_INIT:
        setDirectionInit();
        discernLineInit();
        state = SET_DIRECTION;
        break;
    case SET_DIRECTION:
        if (setDirection()) {
            state = SERCH_LINE_INIT;
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

bool ReturnLine::searchLine() {
    float curDir = rotationCalculator->getRotationAngle();
    float curDis = distanceCalculator->getDistance();

    switch(serchLineState) {
    case SERCH_L:
        switch(serchLineActionState) {
        case SERCH_TURN:
            balancer.control(0, SERCH_LINE_MOVE_POWER);
            if(curDir <= (SERCH_LINE_TURN_DIR * -1.0)) {
                discernLineInit();
                serchLineActionState = SERCH_MOVE;
            }
            break;
        case SERCH_MOVE:
            balancer.control(SERCH_LINE_MOVE_POWER, 0);
            if(curDis >= SERCH_LINE_MOVE_DIS * serchLineCount) {
                discernLineInit();
                serchLineActionState = SERCH_MOVE;
                serchLineState = BACK_L;
            }
            break;
        }
        break;
    case BACK_L:
        switch(serchLineActionState) {
        case SERCH_MOVE:
            balancer.control(-SERCH_LINE_MOVE_POWER, 0);
            if(curDis <= 0.0) {
                discernLineInit();
                serchLineActionState = SERCH_TURN;
            }
            break;
        case SERCH_TURN:
            balancer.control(0, SERCH_LINE_MOVE_POWER);
            if(curDir >= 0.0) {
                discernLineInit();
                serchLineActionState = SERCH_TURN;
                serchLineState = SERCH_R;
            }
            break;
        }
        break;
    case SERCH_R:
        switch(serchLineActionState) {
        case SERCH_TURN:
            balancer.control(0, -SERCH_LINE_MOVE_POWER);
            if(curDir >= SERCH_LINE_TURN_DIR) {
                discernLineInit();
                serchLineActionState = SERCH_MOVE;
            }
            break;
        case SERCH_MOVE:
            balancer.control(SERCH_LINE_MOVE_POWER, 0);
            if(curDis >= SERCH_LINE_MOVE_DIS * serchLineCount) {
                discernLineInit();
                serchLineActionState = SERCH_MOVE;
                serchLineState = BACK_R;
            }
            break;
        }
        break;
    case BACK_R:
        switch(serchLineActionState) {
        case SERCH_MOVE:
            balancer.control(-SERCH_LINE_MOVE_POWER, 0);
            if(curDis <= 0.0) {
                discernLineInit();
                serchLineActionState = SERCH_TURN;
            }
            break;
        case SERCH_TURN:
            balancer.control(0, -SERCH_LINE_MOVE_POWER);
            if(curDir <= 0.0) {
                discernLineInit();
                serchLineActionState = SERCH_TURN;
                serchLineState = SERCH_L;
                serchLineCount++;
            }
            break;
        }
        break;
    default:
        break;
    }

    if (discernLine()) {
        if (serchLineState == SERCH_L) {
            setDirectionState = TURN_R;
        } else {
            setDirectionState = TURN_L;
        }
        return true;
    } else {
        return false;
    }
}

bool ReturnLine::setDirection() {
    float curDir = rotationCalculator->getRotationAngle();

    switch(setDirectionState) {
    case TURN_R:
        balancer.control(0, SET_DIRECTION_MOVE_POWER);
        if (discernLine()) {
            setDirectionRdir = curDir - setDirectionPredir;
            setDirectionPredir = curDir;
            discernLineInit();
            setDirectionState = TURN_L;
        }
        break;
    case TURN_L:
        balancer.control(0, -SET_DIRECTION_MOVE_POWER);
        if (discernLine()) {
            setDirectionLdir = setDirectionPredir - curDir;
            setDirectionPredir = curDir;
            discernLineInit();
            setDirectionState = TURN_R;
        }
        break;
    default:
        break;
    }

    if (edge == true) {
        if (setDirectionLdir < SET_DIRECTION_DIR_DIFF) {
            return true;
        }
    } else {
        if (setDirectionRdir < SET_DIRECTION_DIR_DIFF) {
            return true;
        }
    }

    return false;
}

bool ReturnLine::discernLine() {
    int colorSensorReflect = colorSensor->getBrightness();

    switch (discernLineState) {
    case DISCERN_WHITE1:
        if (colorSensorReflect >= WHITE) {
            discernLineState = DISCERN_BLACK;
        }
        break;
    case DISCERN_BLACK:
        if (colorSensorReflect <= BLACK) {
            discernLineState = DISCERN_WHITE2;
        }
        break;
    case DISCERN_WHITE2:
        if (colorSensorReflect >= WHITE) {
            discernLineState = DISCERN_WHITE1;
            return true;
        }
        break;
    default:
        break;
    }

    return false;
}

void ReturnLine::discernLineInit() {
    discernLineState = DISCERN_WHITE1;
}

void ReturnLine::setDirectionInit() {
    setDirectionPredir = rotationCalculator->getRotationAngle();
    setDirectionLdir = SET_DIRECTION_DIR_DIFF;
    setDirectionRdir = SET_DIRECTION_DIR_DIFF;
}
