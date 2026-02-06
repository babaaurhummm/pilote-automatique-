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

void log_monitor_status();

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
  Monitor.begin();
  Monitor.println("[SETUP] full_unmanned_mode=true");
  com.init();
  rudder.init();
  Monitor.println("[SETUP] COM and RUDDER initialized");
}

void loop() {
  rudder.set_rudder_angle(com.get_com_rudder());
  log_monitor_status();
  wait_until_next_loop();
}

#else

void setup() {
  Monitor.begin();
  Monitor.println("[SETUP] booting...");
  delay(20000);
  Bridge.begin();
  Monitor.println("[SETUP] Bridge started");
  com.init();
  imu.init();
  gps.init();
  rudder.init();
  windsensor.init();
  data.init();
  Monitor.println("[SETUP] modules initialized");
}

void loop() {
  
  update();
  save_data();
  log_monitor_status();
  
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

void log_monitor_status() {
  static unsigned long last_log_ms = 0;
  const unsigned long now = millis();
  if (now - last_log_ms < 1000) {
    return;
  }

  last_log_ms = now;
  Monitor.print("[LOOP] t=");
  Monitor.print(now);
  Monitor.print(" heading=");
  Monitor.print(imu.get_heading());
  Monitor.print(" rudder=");
  Monitor.print(rudder.get_rudder_angle());
  Monitor.print(" unmanned=");
  Monitor.println(com.is_unmanned() ? "1" : "0");
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
  float heading = imu.get_heading();
  float r_angle = rudder.get_rudder_angle();
  float awa = windsensor.get_awa();
  float f_awa = windsensor.get_filtered_awa();
  bool unmanned_status = com.is_unmanned();
  data.save_data(millis(), point.lat, point.lng, heading, r_angle, awa, f_awa, unmanned_status);
}

#if awa_follow_mode

int rudder_angle_sp(int awa_sp) {
  const float awa = windsensor.get_filtered_awa();
  const float heading = imu.get_heading();
  const float heading_sp = awa_sp - awa + heading ;
  const float e = heading_sp - heading ;
  return Kp*e;
  
}

#else

int rudder_angle_sp(int heading_sp) {
  const float heading = imu.get_heading();
  const float e = heading_sp - heading ;
  return Kp*e;
}

#endif

#endif
