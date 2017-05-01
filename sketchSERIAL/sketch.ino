/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <XBOXRECV.h>
#include "ScrapController.h"
#include "ControlWrapper.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#define CHECK_ZONE 5000
const long deadzone = 12000;

USB Usb;
XBOXRECV Xbox(&Usb);

bool leftWasMoving = false;
bool rightWasMoving = false;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
/*#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif*/
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
  Serial1.write("1f9\r");
  delayMicroseconds(1000);
  Serial1.write("2f9\r");
  delay(1000);
  Serial1.write("1f0\r");
  delayMicroseconds(1000);
  Serial1.write("2f0\r");
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
          //Xbox.setRumbleOn(Xbox.getButtonPress(L2, i), Xbox.getButtonPress(R2, i), i);
        }
        long leftYVal = Xbox.getAnalogHat(LeftHatY, i);
        long rightYVal = Xbox.getAnalogHat(RightHatY, i);
        if (abs(leftYVal) > CHECK_ZONE || abs(rightYVal) > CHECK_ZONE) {
          Serial.print(leftYVal);
          Serial.print(" ");
          Serial.println(rightYVal);
          /*if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500) {
            Serial.print(F("LeftHatX: "));
            Serial.print(Xbox.getAnalogHat(LeftHatX, i));
            Serial.print("\t");
          }*/
          if (abs(rightYVal) > deadzone) {
            if (!rightWasMoving) {
              delay(1);
              if (rightYVal > deadzone) {
                Serial1.write("2f9\r");
                Serial.println("2f9\r");
              }
              else if (rightYVal < -deadzone) {
                Serial1.write("2r9\r");
                Serial.println("2r9\r");
              }
              rightWasMoving = true;
            }
          }
          else {
            if (rightWasMoving) {
              delay(1);
              Serial1.write("2f0\r");
              Serial.println("2f0\r");
              rightWasMoving = false;
            }
          }
          if (abs(leftYVal) > deadzone) {
            if (!leftWasMoving) {
              delay(1);
              if (leftYVal > deadzone) {
                Serial1.write("1f9\r");
                Serial.println("1f9\r");
              }
              else if (leftYVal < -deadzone) {
                Serial1.write("1r9\r");
                Serial.println("1r9\r");
              }
              leftWasMoving = true;
            }
            //Serial.print(F("LeftHatY: "));
            //Serial.print(Xbox.getAnalogHat(LeftHatY, i));
            //Serial.print("\t");
          }
          else {
            if (leftWasMoving) {
              delay(1);
              Serial1.write("1f0\r");
              Serial.println("1f0\r");
              leftWasMoving = false;
            }
          }
          /*if (Xbox.getAnalogHat(RightHatX, i) > 7500 || Xbox.getAnalogHat(RightHatX, i) < -7500) {
            Serial.print(F("RightHatX: "));
            Serial.print(Xbox.getAnalogHat(RightHatX, i));
            Serial.print("\t");
          }*/
          
          //Serial.println();
        }
        else {
          if (leftWasMoving) {
            delay(1);
            Serial1.write("1f0\r");
            Serial.println("1f0\r");
            leftWasMoving = false;
          }
          if (rightWasMoving) {
            delay(1);
            Serial1.write("2f0\r");
            Serial.println("2f0\r");
            rightWasMoving = false;
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
        if (Xbox.getButtonClick(X, i)) {
          //Serial.println(F("X"));
          delay(1);
          Serial1.write("1f0\r");
          Serial.println("1f0\r");
          delay(1);
          Serial1.write("2f0\r");
          Serial.println("2f0\r");
          leftWasMoving = false;
          rightWasMoving = false;
        }
          
        if (Xbox.getButtonClick(Y, i))
          Serial.println(F("Y"));
      }
    }
  }
}
