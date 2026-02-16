#include "COM.h"
#include "IMU.h"
#include "GPS.h"
#include "RUDDER.h"
#include "WINDSENSOR.h"
#include "DATA.h"
#include "CONFIG.h"
#include <Arduino_RouterBridge.h>

COM com;
IMU imu;
GPS gps;
RUDDER rudder;
WINDSENSOR windsensor;
DATA data;

const unsigned long loop_period_ms = 1000UL / loop_frequency_hz;

void wait_until_next_loop() {
  static unsigned long last_loop_start = millis();
  const unsigned long now = millis();
  const unsigned long elapsed = now - last_loop_start;

  if (elapsed < loop_period_ms) {
    delay(loop_period_ms - elapsed);
  }

  last_loop_start = millis();
}

#if full_unmanned_mode

void setup() {
  com.init();
  rudder.init();
}

void loop() {
  rudder.set_rudder_angle(com.get_com_rudder());
  wait_until_next_loop();
}

#else

void setup() {
  Bridge.begin();
  Monitor.begin();
  delay(1000);
  com.init();
  imu.init();
  gps.init();
  rudder.init();
  windsensor.init();
  data.init();
}

void loop() {
  
  update();
  save_data();
  
  if (com.is_unmanned())
  {
    #if awa_follow_mode
    rudder.set_rudder_angle(rudder_angle_sp(awa_sp));
    #else
    rudder.set_rudder_angle(rudder_angle_sp(heading_sp)) ;
    #endif
  }
  else
  {
    rudder.set_rudder_angle(com.get_com_rudder());
  }

  wait_until_next_loop();
}

void update() {
  com.update();
  imu.update();
  gps.update();
  windsensor.update();
  rudder.update();
}

void save_data() {
  GPScoord point = gps.get_point();
  const uint16_t heading = imu.get_heading();
  const int16_t r_angle = rudder.get_rudder_angle();
  uint16_t awa = windsensor.get_awa();
  bool unmanned_status = com.is_unmanned();
  data.save_data(millis(), point.lat, point.lng, heading, r_angle, awa, unmanned_status);
}

#if awa_follow_mode

int rudder_angle_sp(int awa_sp) {
  const uint16_t awa = windsensor.get_awa();
  const int16_t e = static_cast<int16_t>(awa_sp) - static_cast<int16_t>(awa);
  return Kp * e;
  
}

#else

int rudder_angle_sp(int heading_sp) {
  const int16_t e = static_cast<int16_t>(heading_sp) - static_cast<int16_t>(imu.get_heading());
  return Kp * e;
}

#endif

#endif