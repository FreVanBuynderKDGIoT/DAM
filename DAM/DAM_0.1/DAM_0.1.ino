#include <Adafruit_ISM330DHCX.h>
#include <Adafruit_LSM6DS.h>
#include <Adafruit_LSM6DS3.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LSM6DSL.h>
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_LSM6DSOX.h>

#include <Adafruit_MPU6050.h>

#include <Adafruit_LSM9DS1.h>


Adafruit_ISM330DHCX ism330dhcx;


void setup() 
{
  Serial.begin(115200);
  while (!Serial)
  delay(10);

  
  if (!ism330dhcx.begin_I2C()) {
    // if (!ism330dhcx.begin_SPI(LSM_CS)) {
    // if (!ism330dhcx.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    Serial.println("Failed to find ISM330DHCX chip");
    while (1) {
      delay(10);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}



void Print6DOF(Adafruit_ISM330DHCX ism330dhcx)
{
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;

  ism330dhcx.getEvent(&accel, &gyro, &temp);

  xSen = accel.acceleration.x;
  ySen = accel.acceleration.y;
  zSen = accel.acceleration.z;

  xAng = map(xSen, minVal, maxVal, -90, 90);
  yAng = map(ySen, minVal, maxVal, -90, 90);
  zAng = map(zSen, minVal, maxVal, -90, 90);

  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  Serial.print("AngleX 6DOF= ");
  Serial.println(x);

  Serial.print("AngleY 6DOF= ");
  Serial.println(y);

  Serial.print("AngleZ 6DOF= ");
  Serial.println(z);
  Serial.println("-----------------------------------------");
}
