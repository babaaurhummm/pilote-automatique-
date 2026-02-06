#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include "CONFIG.h"
#include "WINDSENSOR.h"

WINDSENSOR::WINDSENSOR() {
    ADC_awa_value = 0;
    awa = 0.0;
    f_awa = 0.0 ;
}


void WINDSENSOR::init() {
    Monitor.println("Initializating WindSensor (20 sec max)");
    Monitor.println("--> Put the WindSensor in motion");
    
    unsigned long startTime = millis();

    while (millis() - startTime < 20000) { //20sec
        Monitor.println(analogRead(PIN_WINDSENSOR));
        delay(50); // lecture toutes les 50 ms
    }

}


void WINDSENSOR::update() {
    ADC_awa_value = analogRead(PIN_WINDSENSOR);
    awa = (float)ADC_awa_value / 1023 * 360; 

    if (EMAfilter){
      f_awa = alpha_EMA * awa + (1-alpha_EMA)*f_awa;
    }
    else {
      f_awa = awa ;
    }
}

float WINDSENSOR::get_awa() {
  return awa;
}

float WINDSENSOR::get_filtered_awa() {
  return f_awa;
}