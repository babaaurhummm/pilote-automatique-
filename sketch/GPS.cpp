#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include "GPS.h"

GPS::GPS() : gps(&Serial1), point(GPS_ORIGIN) {}

void GPS::init() {
    Monitor.println("Initializing GPS...");
    gps.begin(9600);
    gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
    gps.sendCommand(PGCMD_ANTENNA);
    delay(1000);
    Monitor.println("Waiting for GPS fix...");
    while (!gps.fix) {
        gps.read();
        if (gps.newNMEAreceived()) {
            gps.parse(gps.lastNMEA());
        }
        delay(100);
    }
    Monitor.println("GPS fix acquired.");
}

void GPS::update() {
    gps.read();

    if (gps.newNMEAreceived()) {
        if (!gps.parse(gps.lastNMEA())) {
            return;
        }
    }

    if (gps.fix) {
        point.lat = gps.latitudeDegrees;
        point.lng = gps.longitudeDegrees;
    }
}

GPScoord GPS::get_point() {
    return point;
}