#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include <Wire.h>
#include "IMU.h"

IMU::IMU() {
    heading = 0u;
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
            const float x = sensorValue.un.magneticField.x;
            const float y = sensorValue.un.magneticField.y;
            const float heading_f = atan2f(y, x) * 180.0f / PI;

            int16_t heading_i = static_cast<int16_t>(heading_f + 0.5f);
            if (heading_i < 0) {
                heading_i += 360;
            }

            if (heading_i >= 360) {
                heading_i -= 360;
            }

            heading = static_cast<uint16_t>(heading_i);
        }
    }
}

uint16_t IMU::get_heading() {
    return heading;
}
