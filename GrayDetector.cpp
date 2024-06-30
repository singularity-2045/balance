#include "GrayDetector.h"

GrayDetector::GrayDetector(ColorSensor* colorSensor) : colorSensor(colorSensor) {}

bool GrayDetector::isGray() const {
    int reflectValue = colorSensor->getReflect();
    return (reflectValue > GRAY_THRESHOLD_LOW && reflectValue < GRAY_THRESHOLD_HIGH);
}