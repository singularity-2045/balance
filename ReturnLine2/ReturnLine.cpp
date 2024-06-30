#include "ReturnLine.h"

ReturnLine::ReturnLine(Balancer* b, ColorSensor* color, RotationCalculator* rotCalc, DistanceCalculator* distCalc, Clock* clk)
    : balancer(b), colorSensor(color), rotationCalculator(rotCalc), distanceCalculator(distCalc), clock(clk), 
      state(SERCH_LINE_INIT), serchState(SERCH_L), directionState(TURN_L), discernState(DISCERN_WHITE1), serchLineCount(1),
      setDirectionLdir(90), setDirectionRdir(90), setDirectionPreDir(0) {}

void ReturnLine::init() {
    state = SERCH_LINE_INIT;
    serchState = SERCH_L;
    directionState = TURN_L;
    discernState = DISCERN_WHITE1;
    serchLineCount = 1;
    initDirection();
    initDiscernLine();
    rotationCalculator->reset();
    distanceCalculator->reset();
}

bool ReturnLine::doReturnLine(bool_t edge) {
    switch (state) {
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
            initDirection();
            initDiscernLine();
            state = SET_DIRECTION;
            break;
        case SET_DIRECTION:
            if (setDirection(edge)) {
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

    switch (serchState) {
        case SERCH_L:
            switch (serchState) {
                case SERCH_TURN:
                    balancer->control(0, 10);
                    if (curDir <= -45.0) {
                        initDiscernLine();
                        serchState = SERCH_MOVE;
                    }
                    break;
                case SERCH_MOVE:
                    balancer->control(10, 0);
                    if (curDis >= 100.0 * serchLineCount) {
                        initDiscernLine();
                        serchState = BACK_L;
                    }
                    break;
            }
            break;
        case BACK_L:
            switch (serchState) {
                case SERCH_MOVE:
                    balancer->control(-10, 0);
                    if (curDis <= 0.0) {
                        initDiscernLine();
                        serchState = SERCH_TURN;
                    }
                    break;
                case SERCH_TURN:
                    balancer->control(0, -10);
                    if (curDir >= 0.0) {
                        initDiscernLine();
                        serchState = SERCH_R;
                    }
                    break;
            }
            break;
        case SERCH_R:
            switch (serchState) {
                case SERCH_TURN:
                    balancer->control(0, -10);
                    if (curDir >= 45.0) {
                        initDiscernLine();
                        serchState = SERCH_MOVE;
                    }
                    break;
                case SERCH_MOVE:
                    balancer->control(10, 0);
                    if (curDis >= 100.0 * serchLineCount) {
                        initDiscernLine();
                        serchState = BACK_R;
                    }
                    break;
            }
            break;
        case BACK_R:
            switch (serchState) {
                case SERCH_MOVE:
                    balancer->control(-10, 0);
                    if (curDis <= 0.0) {
                        initDiscernLine();
                        serchState = SERCH_TURN;
                    }
                    break;
                case SERCH_TURN:
                    balancer->control(0, 10);
                    if (curDir <= 0.0) {
                        initDiscernLine();
                        serchState = SERCH_L;
                        serchLineCount++;
                    }
                    break;
            }
            break;
        default:
            break;
    }

    if (discernLine()) {
        if (serchState == SERCH_L) {
            directionState = TURN_R;
        } else if (serchState == SERCH_R) {
            directionState = TURN_L;
        }
        return true;
    } else {
        return false;
    }
}

bool ReturnLine::setDirection(bool_t edge) {
    float curDir = rotationCalculator->getRotationAngle();

    switch (directionState) {
        case TURN_R:
            balancer->control(0, 5);
            if (discernLine()) {
                setDirectionRdir = curDir - setDirectionPreDir;
                setDirectionPreDir = curDir;
                initDiscernLine();
                directionState = TURN_L;
            }
            break;
        case TURN_L:
            balancer->control(0, -5);
            if (discernLine()) {
                setDirectionLdir = setDirectionPreDir - curDir;
                setDirectionPreDir = curDir;
                initDiscernLine();
                directionState = TURN_R;
            }
            break;
        default:
            break;
    }

    if (edge == L_EDGE) {
        if (setDirectionLdir < 90) {
            return true;
        }
    } else if (edge == R_EDGE) {
        if (setDirectionRdir < 90) {
            return true;
        }
    }

    return false;
}

bool ReturnLine::discernLine() {
    int color = colorSensor->getBrightness();

    switch (discernState) {
        case DISCERN_WHITE1:
            if (color >= WHITE) {
                discernState = DISCERN_BLACK;
            }
            break;
        case DISCERN_BLACK:
            if (color <= BLACK) {
                discernState = DISCERN_WHITE2;
            }
            break;
        case DISCERN_WHITE2:
            if (color >= WHITE) {
                discernState = DISCERN_WHITE1;
                return true;
            }
            break;
        default:
            break;
    }

    return false;
}

void ReturnLine::initDirection() {
    setDirectionPreDir = rotationCalculator->getRotationAngle();
    setDirectionLdir = 90;
    setDirectionRdir = 90;
}

void ReturnLine::initDiscernLine() {
    discernState = DISCERN_WHITE1;
}
