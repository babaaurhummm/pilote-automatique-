#ifndef CONFIG_H
#define CONFIG_H

struct GPScoord {
    float lat;
    float lng;
};

constexpr GPScoord GPS_ORIGIN = {0.0f, 0.0f};

constexpr uint16_t PIN_COM_RUDDER = 2;
constexpr uint16_t PIN_MODE_CONTROL = 3;

constexpr uint16_t pwm_mode_treshold = 1500;
constexpr uint16_t pwm_rudder_center = 1500;
constexpr uint16_t com_pulse_timeout_us = 3000;

constexpr uint16_t ADC_rudder_center = 512;
constexpr uint16_t ADC_rudder_range = 1023;
constexpr uint16_t rudder_max_angle = 30;

constexpr uint16_t max_pulse_rudder = 600;
constexpr uint16_t min_pulse_rudder = 150;

constexpr uint16_t PIN_WINDSENSOR = A0;
constexpr uint16_t PIN_RUDDER_ADC = A1;
constexpr uint16_t RUDDER_CHANNEL = 0;

constexpr int16_t Kp = 1;

constexpr uint16_t loop_frequency_hz = 25;
static_assert(loop_frequency_hz >= 20 && loop_frequency_hz <= 50,
              "loop_frequency_hz must stay between 20 and 50 Hz");

constexpr bool full_unmanned_mode = false ;
constexpr bool awa_follow_mode = false; //true : suivi de d'allure ; false : suivi de cap

constexpr uint16_t heading_sp = 0;
constexpr int16_t awa_sp = 0;


#endif
