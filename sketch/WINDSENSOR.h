#ifndef WINDSENSOR_H
#define WINDSENSOR_H

class WINDSENSOR {
public:
    WINDSENSOR();
    void init();
    void update();
    uint16_t get_awa() ; //angle vent apparent
private:   
    uint16_t awa ; //angle vent apparent (degré)
    uint16_t ADC_awa_value; //valeur retournée par l'ADC (0-1023)
};
#endif