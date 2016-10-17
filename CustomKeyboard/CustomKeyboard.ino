#include <hidboot.h>
#include <usbhub.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

class KbdRptParser : public KeyboardReportParser
{
  protected: virtual void OnKeyDown	(uint8_t mod, uint8_t key);
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  if (key == 40) {
    Serial.println("");
  } else if(key == 42) {
    Serial.print("*Backspace*");
  } else if (key <= 127) {
    uint8_t ascii = OemToAscii(mod, key);
    Serial.print((char)ascii);
  }
}

USB     Usb;
//If the keyboard has a hub (Apple keyboard) then combine them
USBHub     Hub1(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    keyboard(&Usb);
KbdRptParser Parser;

void setup()
{
  //////////////////
  //Debug purpose//
  //////////////////
  Serial.begin( 115200 );
  while (!Serial); //Wait for serial to be open
  Serial.println("Starting...");
  if (Usb.Init() == -1) Serial.println("OSC did not start.");
  //////////////////

  keyboard.SetReportParser(0, (HIDReportParser*)&Parser);
}

void loop()
{
  Usb.Task();
}

