//#include <Mouse2.h>
#include <Mouse.h>
#include "hidmouserptparser.h"

HIDMouseReportParser::HIDMouseReportParser(HIDMouseEvents *evt) : mouEvents(evt), oldButtons(0) {}

void HIDMouseReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
    /* // see all 8 bits of data
    for (uint8_t i = 0; i < len; i++) {
          if(i > 0) Serial.print("\t");
          Serial.print(buf[i], BIN);
          Serial.print(",");
    }
    Serial.println("");
    for (uint8_t i = 0; i < len; i++) {
          if(i > 0) Serial.print("\t");
          Serial.print(buf[i], DEC);
          Serial.print(",");
    }
    Serial.println("");
    Serial.println("");
    */
  //but_id 1,2,4,8,16

  for (uint8_t but_id = 1; but_id <= 16; but_id <<= 1) {
    if (buf[0] & but_id) {
      if (!(oldButtons & but_id)) {
        mouEvents->OnButtonDn(but_id);
      }
    } else {
      if (oldButtons & but_id) {
        mouEvents->OnButtonUp(but_id);
      }
    }
  }
  oldButtons = buf[0];

  xm = 0;
  ym = 0;
  scr = 0;
  tilt = 0;

  xbrute = buf[2];  // Modify based on your mouse
  ybrute = buf[4];  // Modify based on your mouse
  // So a friendly notice, if your mouse move like switched x & y axis, you need to change to follows:
  // xbrute = buf[4];
  // ybrute = buf[2];

  scr = buf[6];  // Modify based on your mouse
  // This is not always 6, different mouse could have different position (for example mine is on 3)
  // Use following if needed
  /*
  if(scr > 127){
    scr = map(scr, 255, 128, -1, -127);
  }
  else if(scr > 0){
    scr = scr*0.99;
    if(scr<1){
      scr = 1;
    }
  }
  */

  tilt = buf[7];  // This could be ignored

  if(xbrute > 127){
    xm = map(xbrute, 255, 128, -1, -127);
  }
  else if(xbrute > 0){
    xm = xbrute*0.99;
    if(xm<1){
      xm = 1;
    }
  }
  if(ybrute > 127){
    ym = map(ybrute, 255, 128, -1, -127);
  }
  else if(ybrute > 0){
    ym = ybrute*0.99;
    if(ym<1){
      ym = 1;
    }
  }

  if ((xm != 0) || (ym != 0) || (scr != 0) || (tilt != 0)) {
    mouEvents->Move(xm, ym, scr, tilt);
  }
}

void HIDMouseEvents::OnButtonDn(uint8_t but_id) {
  Mouse.press(but_id);
}

void HIDMouseEvents::OnButtonUp(uint8_t but_id) {
  Mouse.release(but_id);
}

void HIDMouseEvents::Move(int8_t xm, int8_t ym, int8_t scr, int8_t tilt) {
  /*
  Serial.print("dx: ");
  Serial.print(xm);
  Serial.print(", dy: ");
  Serial.println(ym);
  */
  Mouse.move(xm, ym, scr);
}
