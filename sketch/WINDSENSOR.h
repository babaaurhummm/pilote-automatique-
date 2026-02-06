#ifndef WINDSENSOR_H
#define WINDSENSOR_H

class WINDSENSOR {
public:
    WINDSENSOR();
    void init();
    void update();
    float get_awa() ; //angle vent apparent
    float get_filtered_awa(); //angle vent apparent filtré
private:   
    float awa ; //angle vent apparent (degré)
    int ADC_awa_value; //valeur retournée par l'ADC (0-1023)

    //Si on remarque que les données sont trop instables on filtre (passe-bas Exponential moving average pour ne pas stocker des données)

    float f_awa; //angle vent apparent filtré (degré)
};
#endif