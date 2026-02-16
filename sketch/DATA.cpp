#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include "DATA.h"

DATA::DATA() = default;

void DATA::init() {
    Monitor.println("Initializing DATA logger...");
}

void DATA::save_data(unsigned long timestamp,
                     float latitude,
                     float longitude,
                     uint16_t heading,
                     int16_t rudder_angle,
                     uint16_t awa,
                     bool unmanned_status) {
    Bridge.notify("log_data",
                  timestamp,
                  latitude,
                  longitude,
                  heading,
                  rudder_angle,
                  awa,
                  unmanned_status);
}