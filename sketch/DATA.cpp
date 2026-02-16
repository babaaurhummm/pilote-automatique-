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
                     float heading,
                     float rudder_angle,
                     float awa,
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