#include <Adafruit_ISM330DHCX.h>
#include <Adafruit_LSM6DS.h>
#include <Adafruit_LSM6DS3.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_LSM6DSL.h>
#include <Adafruit_LSM6DSO32.h>
#include <Adafruit_LSM6DSOX.h>




/*
   Project DAM
   Description: This is a test for the mpu6050 sensor to get an angle out of it
   Author: Project DAM
   Date: 09/05/2021
*/

//#include <I2Cdev.h>
#include <Adafruit_MPU6050.h>
#include "math.h"
#include <ISM330DLCSensor.h>
#include <ISM330DLC_ACC_GYRO_Driver.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

#define PI 3.1415926535897932384626433832795
#define RAD_TO_DEG 57.295779513082320876798154814105

// For SPI mode, we need a CS pin
#define LSM_CS 10
// For software-SPI mode we need SCK/MOSI/MISO pins
#define LSM_SCK 13
#define LSM_MISO 12
#define LSM_MOSI 11
#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

Adafruit_MPU6050 accelgyro;
Adafruit_ISM330DHCX ism330dhcx;
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
char xSen, ySen, zSen;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t xAng, yAng, zAng;

double x, y, z;

int minVal = 265;
int maxVal = 402;

#define LED_PIN D7

bool blinkState = false;

void setupSensor()
{
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
}

// setup() runs once, when the device is first turned on.
void setup()
{
  Wire.begin();

  Serial.begin(38400);

  Serial.println("Init I2C device...");
  while (!accelgyro.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
  }

  accelgyro.setAccelerometerRange(MPU6050_RANGE_8_G);
  accelgyro.setGyroRange(MPU6050_RANGE_500_DEG);
  accelgyro.setFilterBandwidth(MPU6050_BAND_21_HZ);


  Serial.println("Adafruit ISM330DHCX test!");

  if (!ism330dhcx.begin_I2C()) {
    // if (!ism330dhcx.begin_SPI(LSM_CS)) {
    // if (!ism330dhcx.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    Serial.println("Failed to find ISM330DHCX chip");
    while (1) {
      delay(10);
    }

    if (!lsm.begin())
    {
      Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
      while (1);
    }
    Serial.println("Found LSM9DS1 9DOF");

    // helper to just set the default scaling we want, see above!
    setupSensor();
  }
}
char choice = 2;

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  switch (choice)
  {
    case 1:
      Print6050(accelgyro);
      break;
    case 2:
      Print6DOF(ism330dhcx);
      break;
    case 3:
      Print9DOF(lsm);
      break;
    case 0:
      break;
  }

  Print6050(accelgyro);
  Print6DOF(ism330dhcx);
  Print9DOF(lsm);
  

  delay(2000);

}

void Print6050(Adafruit_MPU6050 accelgyro)
{
  sensors_event_t a, g, temp;
  accelgyro.getEvent(&a, &g, &temp);

  xSen = a.acceleration.x;
  ySen = a.acceleration.y;
  zSen = a.acceleration.z;

  xAng = map(ax, minVal, maxVal, -90, 90);
  yAng = map(ay, minVal, maxVal, -90, 90);
  zAng = map(az, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  Serial.print("AngleX MPU6050= ");
  Serial.println(x);

  Serial.print("AngleY MPU6050= ");
  Serial.println(y);

  Serial.print("AngleZ MPU6050= ");
  Serial.println(z);
  Serial.println("-----------------------------------------");


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

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  Serial.print("AngleX 6DOF= ");
  Serial.println(x);

  Serial.print("AngleY 6DOF= ");
  Serial.println(y);

  Serial.print("AngleZ 6DOF= ");
  Serial.println(z);
  Serial.println("-----------------------------------------");

}

void Print9DOF(Adafruit_LSM9DS1 lsm)
{
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  xSen  = a.acceleration.x;
  ySen  = a.acceleration.y;
  zSen  = a.acceleration.z;

  xAng = map(xSen, minVal, maxVal, -90, 90);
  yAng = map(ySen, minVal, maxVal, -90, 90);
  zAng = map(zSen, minVal, maxVal, -90, 90);

  x = RAD_TO_DEG * (atan2(-yAng, -zAng) + PI);
  y = RAD_TO_DEG * (atan2(-xAng, -zAng) + PI);
  z = RAD_TO_DEG * (atan2(-yAng, -xAng) + PI);

  Serial.print("AngleX 9DOF= ");
  Serial.println(x);

  Serial.print("AngleY 9DOF= ");
  Serial.println(y);

  Serial.print("AngleZ 9DOF= ");
  Serial.println(z);
  Serial.println("-----------------------------------------");
}
