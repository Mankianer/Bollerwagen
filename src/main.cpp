#include <Wire.h>
#include <MPU6050.h>

#define MOTOR_PIN 9

#define DEBUG 1

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  #if DEBUG
  Serial.println("Start - With Debug");
  #else
  Serial.println("Start - Without Debug");
  #endif

  Wire.begin();
  mpu.initialize();
  pinMode(MOTOR_PIN, OUTPUT); // Motorsteuerung
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float pitch = atan2(ay, sqrt(ax * ax + az * az)) * 180.0 / PI; // TODO ggfs. glätten, um starkes Ruckeln abzufangen
  int motorSpeed = map(pitch, 0, 90, 0, 255); // Anpassen je nach gewünschtem Neigungsbereich
  #if DEBUG
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" Motor Speed: ");
  Serial.println(motorSpeed);
  #endif

  analogWrite(MOTOR_PIN, motorSpeed); // Motorsteuerung an Pin 9
  delay(50);
}