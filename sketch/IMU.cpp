#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include <Wire.h>
#include "IMU.h"

IMU::IMU() {
    heading = 0.0f;
    configured = false;
}

bool IMU::init() {
    Monitor.println("Initializating IMU (BNO085)");
    if (!bno08x.begin_I2C(BNO08x_I2CADDR_DEFAULT,&Wire1)) {
        Monitor.println("BNO085 non detecte");
        configured = false;
        return false;
    }

    if (!bno08x.enableReport(SH2_MAGNETIC_FIELD_CALIBRATED)) {
        Monitor.println("Activation du magnetometre impossible");
        configured = false;
        return false;
    }

    configured = true;
    return true;
}

void IMU::update() {
    if (!configured) {
        return;
    }

    if (bno08x.getSensorEvent(&sensorValue)) {
        if (sensorValue.sensorId == SH2_MAGNETIC_FIELD_CALIBRATED) {
            float x = sensorValue.un.magneticField.x;
            float y = sensorValue.un.magneticField.y;
            heading = atan2(y, x) * 180.0f / PI;
            if (heading < 0.0f) {
                heading += 360.0f;
            }
        }
    }
}

uint16_t IMU::get_heading() {
    return heading;
}