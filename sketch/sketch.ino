/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <XBOXRECV.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#define OUT1_1 40
#define OUT1_2 42
#define PWM_1 44
#define OUT2_1 41
#define OUT2_2 43
#define PWM_2 45

#define Enc1_A 18 //interrupt!
#define Enc1_B 16 //normal digital!


bool isMoving1 = false;
bool wasMoving1 = isMoving1;
bool isMoving2 = false;
int deadzone = 12000;
int checkzone = 5000;

//represents encoder cound
volatile int count1;
volatile byte PinA1Last;
volatile boolean Direction1;


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
  //encoder initialization
  initEncoders();
  
  //setup display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();
  showText("Encoder test!");
  
}

void initEncoders() {
  pinMode(Enc1_A, INPUT);
  pinMode(Enc1_B, INPUT);
  count1 = 0;
  attachInterrupt(digitalPinToInterrupt(Enc1_A),Encoder1Function, CHANGE); 
}

void Encoder1Function() {
  if (digitalRead(Enc1_A) == digitalRead(Enc1_B)) {
    count1++;
  }
  else {
    count1--;
  }
}

void wheelSpeed1() {
  int Lstate = digitalRead(Enc1_A);
  if((PinA1Last == LOW) && Lstate == HIGH)
  {
    int val = digitalRead(Enc1_B);
    if (val == LOW && Direction1)
    {
      Direction1 = false;
    }
    else if (val == HIGH && !Direction1)
    {
      Direction1 = true;
    }
  }
  PinA1Last = Lstate;
  if (!Direction1) count1++;
  else count1--;
}


void showText(String text) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(text);
  display.display();
  display.clearDisplay();
}

void loop() {
  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        if (Xbox.getButtonPress(L2, i) || Xbox.getButtonPress(R2, i)) {
          Serial.print("L2: ");
          Serial.print(Xbox.getButtonPress(L2, i));
          Serial.print("\tR2: ");
          Serial.println(Xbox.getButtonPress(R2, i));
          Xbox.setRumbleOn(Xbox.getButtonPress(L2, i), Xbox.getButtonPress(R2, i), i);
        }

        if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500 || Xbox.getAnalogHat(LeftHatY, i) > checkzone || Xbox.getAnalogHat(LeftHatY, i) < -checkzone || Xbox.getAnalogHat(RightHatX, i) > checkzone || Xbox.getAnalogHat(RightHatX, i) < -checkzone || Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
          if (Xbox.getAnalogHat(LeftHatX, i) > 7500 || Xbox.getAnalogHat(LeftHatX, i) < -7500) {
            //Serial.print(F("LeftHatX: "));
            //Serial.print(Xbox.getAnalogHat(LeftHatX, i));
            //Serial.print("\t");
          }
          //throttle HERE!
          if (Xbox.getAnalogHat(LeftHatY) > deadzone || Xbox.getAnalogHat(LeftHatY) < -deadzone) {
            //Serial.print(F("LeftHatX: "));
            //Serial.print(Xbox.getAnalogHat(LeftHatX));
            //Serial.print("\t");
            if (Xbox.getAnalogHat(LeftHatY) > deadzone) {
              digitalWrite(OUT1_1,HIGH);
              digitalWrite(OUT1_2,LOW);
              analogWrite(PWM_1,80+map(abs(Xbox.getAnalogHat(LeftHatY)),deadzone,32768,0,175));
              isMoving1 = true;
              if (wasMoving1 != isMoving1) {
                showText("movingF");
                wasMoving1 = isMoving1;
              }
            }
            else if (Xbox.getAnalogHat(LeftHatY) < -deadzone) {
              digitalWrite(OUT1_1,LOW);
              digitalWrite(OUT1_2,HIGH);
              analogWrite(PWM_1,80+map(abs(Xbox.getAnalogHat(LeftHatY)),deadzone,32768,0,175));
              isMoving1 = true;
              if (wasMoving1 != isMoving1) {
                showText("movingB");
                wasMoving1 = isMoving1;
              }
            }
          }
          else {
            //analogWrite(PWM_1,0);
            digitalWrite(OUT1_1,LOW);
            digitalWrite(OUT1_2,LOW);
            isMoving1 = false;
            if (wasMoving1 != isMoving1) {
                showText("off");
                wasMoving1 = isMoving1;
              }
          }
          //turning HERE!
          if (Xbox.getAnalogHat(RightHatX, i) > deadzone || Xbox.getAnalogHat(RightHatX, i) < -deadzone) {
            //Serial.print(F("RightHatX: "));
            //Serial.print(Xbox.getAnalogHat(RightHatX, i));
            //Serial.print("\t");if (Xbox.getAnalogHat(LeftHatY) > deadzone) {
            if (Xbox.getAnalogHat(RightHatX) > deadzone) {
              digitalWrite(OUT2_1,HIGH);
              digitalWrite(OUT2_2,LOW);
              analogWrite(PWM_2,80+map(abs(Xbox.getAnalogHat(RightHatX)),deadzone,32768,0,175));
              isMoving2 = true;
            }
            else if (Xbox.getAnalogHat(RightHatX) < -deadzone) {
              digitalWrite(OUT2_1,LOW);
              digitalWrite(OUT2_2,HIGH);
              analogWrite(PWM_2,80+map(abs(Xbox.getAnalogHat(RightHatX)),deadzone,32768,0,175));
              isMoving2 = true;
            }
          }
          else {
            analogWrite(PWM_2,0);
            isMoving2 = false;
          }
          if (Xbox.getAnalogHat(RightHatY, i) > 7500 || Xbox.getAnalogHat(RightHatY, i) < -7500) {
            //Serial.print(F("RightHatY: "));
            //Serial.print(Xbox.getAnalogHat(RightHatY, i));
          }
          //Serial.println();
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
          //Serial.println(F("B"));
          showText(String(count1));
        if (Xbox.getButtonClick(X, i))
          //Serial.println(F("X"));
          analogWrite(PWM_1,0);
          analogWrite(PWM_2,0);
          isMoving1 = false;
          isMoving2 = false;
        if (Xbox.getButtonClick(Y, i))
          Serial.println(F("Y"));
      }
    }
  }
}
