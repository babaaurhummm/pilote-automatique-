#ifndef DATA_H
#define DATA_H

class DATA {
public:
    DATA();
    void init();
    void save_data(unsigned long timestamp,
                   float latitude,
                   float longitude,
                   float heading,
                   float rudder_angle,
                   float awa,
                   float filtered_awa,
                   bool unmanned_status);
};

#endif