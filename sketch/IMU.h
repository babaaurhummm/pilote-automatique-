#ifndef IMU_H
#define IMU_H

#include <Adafruit_BNO08x.h>

class IMU {
public:
    IMU();
    bool init();
    void update();
    float get_heading(); // renvoie l'angle fait avec le nord magnétique en degrés

private:
    Adafruit_BNO08x bno08x;
    sh2_SensorValue_t sensorValue;
    float heading;
    bool configured;
};

#endif