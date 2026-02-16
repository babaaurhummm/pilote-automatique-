#ifndef WINDSENSOR_H
#define WINDSENSOR_H

#include <Arduino.h>

class WINDSENSOR {
public:
    WINDSENSOR();
    void init();
    void update();
    int16_t get_awa(); // angle vent apparent [-180; 180]
private:
    int16_t awa;          // angle vent apparent (degre), + droite / - gauche, 0 arriere
    uint16_t ADC_awa_value; // valeur retournee par l'ADC (0-1023)
};
#endif
