#include "ZigZagLineTracer.h"

ZigZagLineTracer::ZigZagLineTracer(Balancer& b, ColorSensor* color, Clock* c)
    : balancer(b), colorSensor(color), clock(c) {
}

void ZigZagLineTracer::traceLine(int speed) {
    while (true) {
        int brightness = colorSensor->getReflect();
        int turn = (brightness < 30) ? 20 : -20; // 라인의 색상에 따라 회전

        balancer.control(speed, turn);
        clock->sleep(4); // 4ms 대기
    }
}
