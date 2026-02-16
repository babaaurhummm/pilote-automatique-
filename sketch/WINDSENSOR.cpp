#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include "CONFIG.h"
#include "WINDSENSOR.h"

WINDSENSOR::WINDSENSOR() {
    ADC_awa_value = 0u;
    awa = 0;
}


void WINDSENSOR::init() {
    delay(5000);
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

    // Convention demandee:
    // - ADC = 512 : vent arriere => awa = 0
    // - ADC = 0   : vent de face cote tribord => awa = +180
    // - ADC = 1023: vent de face cote babord  => awa = -180
    if (ADC_awa_value <= 512u) {
        const uint32_t delta = 512u - ADC_awa_value;
        const uint32_t tmp = delta * 180u + 256u; // arrondi /512
        awa = static_cast<int16_t>(tmp / 512u);
    } else {
        const uint32_t delta = ADC_awa_value - 512u;
        const uint32_t tmp = delta * 180u + 255u; // arrondi /511
        awa = -static_cast<int16_t>(tmp / 511u);
    }
}


int16_t WINDSENSOR::get_awa() {
  return awa;
}
