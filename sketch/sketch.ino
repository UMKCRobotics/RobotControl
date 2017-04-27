/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <XBOXRECV.h>
#include "ScrapController.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif


#define OUT1_1 40
#define OUT1_2 42
#define PWM_1 44
#define OUT2_1 41
#define OUT2_2 43
#define PWM_2 46

ScrapMotor motor1(OUT1_1,OUT1_2,PWM_1);
ScrapMotor motor2(OUT2_1,OUT2_2,PWM_2);


#define MAX_PWM 255
#define MIN_PWM 80


bool isMoving1 = false;
bool wasMoving1 = isMoving1;
bool isMoving2 = false;
bool wasMoving2 = isMoving2;
int deadzone = 12000;
int checkzone = 5000;


USB Usb;
XBOXRECV Xbox(&Usb);

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
  motor1.setMotor(255);
  motor2.setMotor(255);
  delay(1000);
  motor1.setMotor(-255);
  motor2.setMotor(-255);
  delay(1000);
  motor1.stop();
  motor2.stop();
  Serial.println(-map(abs(-32768),deadzone,32768,MIN_PWM,MAX_PWM));
              
  
}

void loop() {
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 1; i++) {
      if (Xbox.Xbox360Connected[i]) {
        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i)) {
          Serial.print("L2: ");
          Serial.print(Xbox.getButtonPress(L2, i));
          Serial.print("\tR2: ");
          Serial.println(Xbox.getButtonPress(R2, i));
          Xbox.setRumbleOn(Xbox.getButtonPress(L2, i), Xbox.getButtonPress(R2, i), i);
        }
        
        if (Xbox.getAnalogHat(RightHatX, i) > checkzone || Xbox.getAnalogHat(RightHatX, i) < -checkzone || Xbox.getAnalogHat(RightHatY, i) > checkzone || Xbox.getAnalogHat(RightHatY, i) < -checkzone) {
          if (Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500) {
            //Serial.print(F("RightHatX: "));
            //Serial.print(Xbox.getAnalogHat(RightHatX, i));
            //Serial.print("\t");
          }
          //throttle HERE!
          if (Xbox.getAnalogHat(RightHatY) > deadzone || Xbox.getAnalogHat(RightHatY) < -deadzone) {
            //Serial.print(F("RightHatX: "));
            //Serial.print(Xbox.getAnalogHat(RightHatX));
            //Serial.print("\t");
            if (Xbox.getAnalogHat(RightHatY) > deadzone) {
              motor2.setMotor(map(abs(Xbox.getAnalogHat(RightHatY)),deadzone,32768,MIN_PWM,MAX_PWM));
              isMoving2 = true;
              if (wasMoving2 != isMoving2) {
                wasMoving2 = isMoving2;
              }
            }
            else if (Xbox.getAnalogHat(RightHatY) < -deadzone) {
              motor2.setMotor(-map(abs(Xbox.getAnalogHat(RightHatY)),deadzone,32768,MIN_PWM,MAX_PWM));
              isMoving2 = true;
              if (wasMoving2 != isMoving2) {
                wasMoving2 = isMoving2;
              }
            }
          }
          else {
            motor2.stop();
            isMoving2 = false;
            if (wasMoving2 != isMoving2) {
              wasMoving2 = isMoving2;
            }
          }
        }
  
        if (Xbox.getAnalogHat(LeftHatX, i) > checkzone || Xbox.getAnalogHat(LeftHatX, i) < -checkzone || Xbox.getAnalogHat(LeftHatY, i) > checkzone || Xbox.getAnalogHat(LeftHatY, i) < -checkzone) {
          if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500) {
            //Serial.print(F("LeftHatX: "));
            //Serial.print(Xbox.getAnalogHat(LeftHatX, i));
            //Serial.print("\t");
          }
          //throttle HERE!
          if (Xbox.getAnalogHat(LeftHatY) > deadzone || Xbox.getAnalogHat(LeftHatY) < -deadzone) {
            //Serial.print(F("LeftHatY: "));
            Serial.println(Xbox.getAnalogHat(LeftHatY));
            //Serial.print("\t");
            if (Xbox.getAnalogHat(LeftHatY) > deadzone) {
              motor1.setMotor(map(abs(Xbox.getAnalogHat(LeftHatY)),deadzone,32768,MIN_PWM,MAX_PWM));
              //motor2.setMotor(map(abs(Xbox.getAnalogHat(LeftHatY)),deadzone,32768,MIN_PWM,MAX_PWM));
              isMoving1 = true;
              if (wasMoving1 != isMoving1) {
                wasMoving1 = isMoving1;
              }
            }
            else if (Xbox.getAnalogHat(LeftHatY) < -deadzone) {
              motor1.setMotor(-map(abs(Xbox.getAnalogHat(LeftHatY)),deadzone,32768,MIN_PWM,MAX_PWM));
              Serial.println(-map(abs(Xbox.getAnalogHat(LeftHatY)),deadzone,32768,MIN_PWM,MAX_PWM));
              //motor2.setMotor(-map(abs(Xbox.getAnalogHat(LeftHatY)),deadzone,32767,MIN_PWM,MAX_PWM));
              isMoving1 = true;
              if (wasMoving1 != isMoving1) {
                wasMoving1 = isMoving1;
              }
            }
          }
          else {
            motor1.stop();
            //motor2.stop();
            isMoving1 = false;
            if (wasMoving1 != isMoving1) {
              wasMoving1 = isMoving1;
            }
          }
        }

        

        if (Xbox.getButtonClick(UP, i)) {
          Xbox.setLedOn(LED1, i);
          Serial.println(F("Up"));
        }
        if (Xbox.getButtonClick(DOWN, i)) {
          Xbox.setLedOn(LED4, i);
          Serial.println(F("Down"));
        }
        if (Xbox.getButtonClick(LEFT, i)) {
          Xbox.setLedOn(LED3, i);
          Serial.println(F("Left"));
        }
        if (Xbox.getButtonClick(RIGHT, i)) {
          Xbox.setLedOn(LED2, i);
          Serial.println(F("Right"));
        }

        if (Xbox.getButtonClick(START, i)) {
          Xbox.setLedMode(ALTERNATING, i);
          Serial.println(F("Start"));
        }
        if (Xbox.getButtonClick(BACK, i)) {
          Xbox.setLedBlink(ALL, i);
          Serial.println(F("Back"));
        }
        if (Xbox.getButtonClick(L3, i))
          Serial.println(F("L3"));
        if (Xbox.getButtonClick(R3, i))
          Serial.println(F("R3"));

        if (Xbox.getButtonClick(L1, i))
          Serial.println(F("L1"));
        if (Xbox.getButtonClick(R1, i))
          Serial.println(F("R1"));
        if (Xbox.getButtonClick(XBOX, i)) {
          Xbox.setLedMode(ROTATING, i);
          Serial.print(F("Xbox (Battery: "));
          Serial.print(Xbox.getBatteryLevel(i)); // The battery level in the range 0-3
          Serial.println(F(")"));
        }
        if (Xbox.getButtonClick(SYNC, i)) {
          Serial.println(F("Sync"));
          Xbox.disconnect(i);
        }

        if (Xbox.getButtonClick(A, i))
          Serial.println(F("A"));
        if (Xbox.getButtonClick(B, i))
          Serial.println(F("B"));
        if (Xbox.getButtonClick(X, i))
          //Serial.println(F("X"));
          motor1.stop();
          motor2.stop();
          isMoving1 = false;
          isMoving2 = false;
        if (Xbox.getButtonClick(Y, i))
          Serial.println(F("Y"));
      }
    }
  }
}
