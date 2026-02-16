#ifndef DATA_H
#define DATA_H

class DATA {
public:
    DATA();
    void init();
    void save_data(unsigned long timestamp,
                   float latitude,
                   float longitude,
                   uint16_t heading,
                   uint16_t rudder_angle,
                   uint16_t awa,
                   bool unmanned_status);
};

#endif