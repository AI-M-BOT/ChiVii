#include <Mouse.h>
#include <hiduniversal.h>
#include <Keyboard.h>
// #include <Wire.h>
#include <SPI.h>
// #include <hidboot.h>
// #include <usbhub.h>
#include "hidmouserptparser.h"

USB Usb;
// USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
HIDMouseEvents MouEvents;
HIDMouseReportParser Mou(&MouEvents);

// HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);
String myString;
String checkString;

const int upButton = 2;
const int downButton = 3;
const int leftButton = 4;
const int rightButton = 5;
const int mouseButton = 6;

int comma;
int ender;

int movex;
int movey;

int key;

int arrv[8];

void setup() {
  delay(5000);
  // pinMode(upButton, INPUT);
  // pinMode(downButton, INPUT);
  // pinMode(leftButton, INPUT);
  // pinMode(rightButton, INPUT);
  // pinMode(mouseButton, INPUT);

  Mouse.begin();
  Keyboard.begin();

  Serial.begin(115200);
  Serial.setTimeout(1);

  if (Usb.Init() == -1)
    Serial.println("Not started.");
  // HidMouse.SetReportParser(0, &Prs);
  if (!Hid.SetReportParser(0, &Mou))
    ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);
}


void loop() {

  key = 0;

  //Moving the mouse with pcinput
  if (Serial.available()) {
    // m(12,46) mousemove
    // p(1)     mouse press
    // r(0)     mouse release
    // d(a)     key down
    // u(a)     key up

    myString = Serial.readString();
    char inChar = myString[0];

    switch (inChar) {
      case 'a':
        if (myString == "arduino") {
          Serial.print("arduinoshield");
          delay(100);
        }
        break;

      case 'm':
        comma = myString.indexOf(',');
        ender = myString.indexOf(')');
        movex = myString.substring(2, comma).toInt();
        movey = myString.substring(comma + 1, ender).toInt();
        arrv[0] = max(-127, min(127, movex));
        arrv[1] = max(-127, min(127, movey));
        arrv[2] = 0;  // Ensure
        Mouse.move(arrv[0], arrv[1], arrv[2]);
        memset(arrv, 0, sizeof(arrv));
        break;

      case 'p':
        key = String(myString[2]).toInt();
        if (key == 1) {
          Mouse.press(MOUSE_LEFT);
        }
        else if (key == 2) {
          Mouse.press(MOUSE_RIGHT);
        }
        break;

      case 'r':
        key = String(myString[2]).toInt();
        if (key == 1) {
          Mouse.release(MOUSE_LEFT);
        }
        else if (key == 2) {
          Mouse.release(MOUSE_RIGHT);
        }
        break;

      case 'd':
        Keyboard.press(myString[2]);
        break;

      case 'u':
        Keyboard.release(myString[2]);
        break;
    }
  } else {
    Usb.Task();
  }
}
