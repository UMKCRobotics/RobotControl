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
const int deadzone = 12000;

#define OUTLEFT_1 41
#define OUTLEFT_2 43
#define PWM_LEFT 45
#define OUTRIGHT_1 40
#define OUTRIGHT_2 42
#define PWM_RIGHT 44

USB Usb;
XBOXRECV Xbox(&Usb);

ScrapMotor leftMotor(OUTLEFT_1,OUTLEFT_2,PWM_LEFT,-1);
ScrapMotor rightMotor(OUTRIGHT_1,OUTRIGHT_2,PWM_RIGHT,-1);
ControlWrapper controls = ControlWrapper();

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
}

void loop() {
  doMovement();
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i)) {
          Serial.print("L2: ");
          Serial.print(Xbox.getButtonPress(L2, i));
          Serial.print("\tR2: ");
          Serial.println(Xbox.getButtonPress(R2, i));
          //Xbox.setRumbleOn(Xbox.getButtonPress(L2, i), Xbox.getButtonPress(R2, i), i);
        }

        // movement code

        long leftYVal = Xbox.getAnalogHat(LeftHatY, i);
        long rightYVal = Xbox.getAnalogHat(RightHatY, i);

        if (abs(leftYVal) > CHECK_ZONE || abs(rightYVal) > CHECK_ZONE) {
          if (abs(rightYVal) > deadzone) {
            controls.setRawRightY(Xbox.getAnalogHat(RightHatY, i));
          }
          else {
            controls.setRawRightY(0);
          }
          if (abs(leftYVal) > deadzone) {
            controls.setRawLeftY(Xbox.getAnalogHat(LeftHatY, i));
          }
          else {
            controls.setRawLeftY(0);
          }
          
        }
        else {
          controls.setRawLeftY(0);
          controls.setRawRightY(0);
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
        // stop all movement
        if (Xbox.getButtonClick(X, i)) {
          controls.setRawLeftY(0);
          controls.setRawRightY(0);
        }

        if (Xbox.getButtonClick(Y, i))
          Serial.println(F("Y"));
      }
    }
  }
}

void doMovement() {
  controls.updateMotorPWM();
  leftMotor.setMotor(controls.getLeftPWM());
  rightMotor.setMotor(controls.getRightPWM());
}