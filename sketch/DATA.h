#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

class DATA {
public:
    DATA();
    void init();
    void save_data(unsigned long timestamp,
                   float latitude,
                   float longitude,
                   uint16_t heading,
                   int16_t rudder_angle,
                   int16_t awa,
                   bool unmanned_status);
};

#endif
