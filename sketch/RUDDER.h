#ifndef RUDDER_H
#define RUDDER_H

#include <Arduino.h>

class RUDDER {
public:
    RUDDER();
    void init();
    void update();
    void set_rudder_angle(int angle);
    int16_t get_rudder_angle();
private:
    int16_t rudder_angle;
    int ADC_rudder_value;
    int16_t k;
    void test_rudder_servo();
};

#endif
