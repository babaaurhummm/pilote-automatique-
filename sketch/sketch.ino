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


#if full_unmanned_mode

void setup() {
  Bridge.begin();
  com.init();
  rudder.init();
}

void loop() {
  rudder.set_rudder_angle(com.get_com_rudder());
}

#else

void setup() {
  Bridge.begin();
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