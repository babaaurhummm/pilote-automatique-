#include <Arduino.h>
#include <Arduino_RouterBridge.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "CONFIG.h"
#include "RUDDER.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

RUDDER::RUDDER() {
  rudder_angle = 0;
  ADC_rudder_value = ADC_rudder_center;
  k = 2.0f * rudder_max_angle / ADC_rudder_range;
}


void RUDDER::init() {
  Monitor.begin();
  Monitor.println("Initializating Adafruit Servo Shield...");

  pwm.begin();
  pwm.setPWMFreq(50);  

  test_rudder_servo();
  
  delay(10);
}


void RUDDER::test_rudder_servo(){

  for (int pulse = min_pulse_rudder; pulse <= max_pulse_rudder; pulse++) {
    pwm.setPWM(RUDDER_CHANNEL, 0, pulse);
    delay(5);
  }

  for (int pulse = max_pulse_rudder; pulse >= min_pulse_rudder; pulse--) {
    pwm.setPWM(RUDDER_CHANNEL, 0, pulse);
    delay(5);
  }
}

void RUDDER::update(){
  ADC_rudder_value = analogRead(PIN_RUDDER_ADC);
  rudder_angle = k * (ADC_rudder_value - ADC_rudder_center);
}

float RUDDER::get_rudder_angle() {
  return rudder_angle;
}
void RUDDER::set_rudder_angle(int angle){
  int max_angle = static_cast<int>(rudder_max_angle);
  int pulse = map(angle, -max_angle, max_angle, min_pulse_rudder, max_pulse_rudder);
  pwm.setPWM(RUDDER_CHANNEL, 0, pulse);
}