#include <hidboot.h>
#include <usbhub.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

class KbdRptParser : public KeyboardReportParser
{
  protected:virtual void OnKeyDown	(uint8_t mod, uint8_t key);
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  if (key == 40) {
    Serial.println("");
  } else if (key <= 127) {
    uint8_t ascii = OemToAscii(mod, key);
    Serial.print((char)ascii);
  }
  //Serial.print("DN ");
  //PrintKey(mod, key);
  //uint8_t c = OemToAscii(mod, key);

  //if (c)
  //OnKeyPressed(c);
}

USB     Usb;
//If the keyboard has a hub (Apple keyboard) then combine them
USBHub     Hub1(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard1(&Usb);

KbdRptParser Prs;

void setup()
{
  Serial.begin( 115200 );
  while (!Serial);
  Serial.println("Starting...");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  HidKeyboard1.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop()
{
  Usb.Task();
}

