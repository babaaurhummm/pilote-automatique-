#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Adafruit_BNO08x.h>

class IMU {
public:
    IMU();
    bool init();
    void update();
    uint16_t get_heading(); // renvoie l'angle fait avec le nord magnétique en degrés

private:
    Adafruit_BNO08x bno08x;
    sh2_SensorValue_t sensorValue;
    uint16_t heading;
    bool configured;
};

#endif
