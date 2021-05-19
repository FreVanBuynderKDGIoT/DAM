/*
 * Project DAM
 * Description: This is a test for the mpu6050 sensor to get an angle out of it
 * Author: Project DAM
 * Date: 09/05/2021
 */

#include <I2Cdev.h>
#include <MPU6050.h>
#include "math.h"

#define PI 3.1415926535897932384626433832795
#define RAD_TO_DEG 57.295779513082320876798154814105

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t xAng, yAng, zAng;

double x, y, z;

int minVal = 265;
int maxVal = 402;

#define LED_PIN D7

bool blinkState = false;

// setup() runs once, when the device is first turned on.
void setup() {
  Wire.begin();

  Serial.begin(38400);

  Serial.println("Init I2C device...");
  accelgyro.initialize();

  Serial.println("testing MPU6050 connection...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  pinMode(LED_PIN, OUTPUT);

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  xAng = map(ax, minVal, maxVal, -90, 90);
  yAng = map(ay, minVal, maxVal, -90, 90);
  zAng = map(az, minVal, maxVal, -90, 90);

  x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
  y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
  z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

  Serial.print("AngleX= ");
  Serial.println(x);
  
  Serial.print("AngleY= ");
  Serial.println(y);
  
  Serial.print("AngleZ= ");
  Serial.println(z);
  Serial.println("-----------------------------------------");
  delay(400);

}