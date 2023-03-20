#include <Wire.h>
#include <MPU6050.h>
#include <stdio.h>
#include <Arduino.h>

#define MOTOR_PIN 9

#define DEBUG 1
#define IS_UNO 1

MPU6050 mpu;

#if IS_UNO
// Eine Funktion, um stdout auf die serielle Schnittstelle umzuleiten
static int uart_putchar(char c, FILE* stream) {
  Serial.write(c);
  return 0;
}
// Eine Dateistruktur, um die serielle Schnittstelle als Ausgabestream zu verwenden
static FILE uart_output = {0};
#endif

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.begin();

  #if DEBUG
    #if IS_UNO
    // Die Ausgabefunktion der Dateistruktur festlegen
    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);

    // stdout auf die serielle Schnittstelle umleiten
    stdout = &uart_output;
    #endif
  Serial.println("Start - With Debug");
  Serial.println("MPU6050 test program status: ");
  while(!mpu.available()) {
    Serial.println("MPU6050 connection failed");
    delay(1000);
  }
  Serial.println("MPU6050 connection successful");
  #else
  Serial.println("Start - Without Debug");
  #endif
  
  // mpu calibrieren
  // set the scaling registers
  mpu.setAccConfig(MPU6050_ACC_FULL_SCALE_2_G);
  mpu.setGyroConfig(MPU6050_GYRO_FULL_SCALE_500_DPS);

  // run the self calibration function, must not move the MPU during this function call!
  mpu.calibrate();
  
  pinMode(MOTOR_PIN, OUTPUT); // Motorsteuerung
}

void loop() {
  mpu.update();
  vector_t gyro = mpu.getGyro();
  attitude_t attitude = mpu.getAttitude(UNITS_DEGREES);

  long motorSpeed = (long)attitude.roll > -20 ? -1 : max(min(map((long)attitude.roll, -60, -45, 0, 255), 255), 0); // Motor  speed 0-255 zwischen -60 und -10 Grad
  #if DEBUG
  Serial.print(attitude.pitch);
  Serial.print(" ");
  Serial.print((long)attitude.roll);
  Serial.print(" ");
  Serial.print(attitude.yaw);
  Serial.print(" ");
  Serial.print(motorSpeed);
  Serial.print(" ");
  Serial.print(motorSpeed > 0 ? "AN" : "AUS");
  Serial.println();
  // printf("x: %4d, y: %4d, z: %4d \n", gyro.x, gyro.y, gyro.z);
  #endif


  // analogWrite(MOTOR_PIN, motorSpeed); // Motorsteuerung an Pin 9
  delay(50);
}