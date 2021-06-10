/*
   Project DAM
   Description: This is a test for the mpu6050 sensor to get an angle out of it
   Author: Project DAM
   Date: 09/05/2021
*/

#include "config.h"

#include <MPU6050_light.h>
#include <Wire.h>

MPU6050 accelgyro(Wire);

// adafruit setup
AdafruitIO_Feed *mpu = io.feed("mpu-6050");
AdafruitIO_Feed *overstort = io.feed("overstort");
AdafruitIO_Feed *vast = io.feed("vast");

double y;
double Py;
bool bOverstort;
bool oSended;
bool pVast;
unsigned long iOverstort;
unsigned long timeZelfdeHoek;

unsigned long treshholdVast = 10;

void SendOverstort(double);
void SendSensors(int);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Wire.begin();

  Serial.begin(38400);

  io.connect();

  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println(io.statusText());

  Serial.println("Init I2C device...");
  byte status = accelgyro.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0)
  {
    Serial.println("not connnected");
  }

  Serial.println(F("While calculating the offsets value, do not move the MPU6050 sensor"));
  delay(1000);
  accelgyro.upsideDownMounting = true; // Comment on this line if MPU6050 is installed backwards
  accelgyro.calcOffsets();
  Serial.println("Done!\n");

  digitalWrite(LED_BUILTIN, HIGH);
}
void loop()
{
  io.run();
  Wire.begin();

  y = Calculate6050(accelgyro);

  if (y >= 5 && oSended == false && bOverstort == false)
  {
    bOverstort = true;
    oSended = true;
    iOverstort = millis();
    if (timeZelfdeHoek >= (treshholdVast * 1000))
    {
      SendVast(true);
    }
    else
    {
      SendVast(false);
    }
  }
  else if (y < 5 && oSended == false && bOverstort == true)
  {
    bOverstort = false;
    SendOverstort(iOverstort);
    oSended = true;
  }
  if (bOverstort == true && Py != y)
  {
    SendSensors(y);
  }

  if (Py != y)
  {
    timeZelfdeHoek = millis();
    Py = y;
  }
  
  delay(2000);
}

double Calculate6050(MPU6050 mpu)
{
  mpu.update();
  y = mpu.getAngleY();
  return y;

}
void SendVast(bool blVast)
{
  if (blVast != pVast)
  {
    vast->save(blVast);
    Serial.println("vast send");
    pVast = vast;
  }
}

void SendOverstort(unsigned long iOverstort)
{
  overstort->save(iOverstort);
   Serial.println("overstort send");
}

void SendSensors(double y)
{
  mpu->save(y);
  Serial.println("sensor send");
}
