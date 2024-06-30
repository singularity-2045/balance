#ifndef GRAY_DETECTOR_H
#define GRAY_DETECTOR_H

#include "ColorSensor.h"

class GrayDetector {
public:
    GrayDetector(ColorSensor* colorSensor);
    bool isGray() const;

private:
    ColorSensor* colorSensor;

    // EV3 색상 센서의 반사광 값에 대한 임계값
    static const int WHITE_THRESHOLD = 90;   // 흰색 임계값
    static const int BLACK_THRESHOLD = 10;   // 검은색 임계값
    static const int GRAY_THRESHOLD_LOW = 30;  // 회색 하한 임계값
    static const int GRAY_THRESHOLD_HIGH = 60; // 회색 상한 임계값
};

#endif // GRAY_DETECTOR_H