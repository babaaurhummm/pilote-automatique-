#ifndef GPS_H
#define GPS_H

#include <Adafruit_GPS.h>
#include "CONFIG.h"

class GPS {
public:
    GPS();
    void init();
    void update();
    GPScoord get_point();
private:
    Adafruit_GPS gps;
    GPScoord point;
};

#endif