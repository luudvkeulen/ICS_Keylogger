#include <hidboot.h>
#include <usbhub.h>
// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    virtual void OnKeyDown	(uint8_t mod, uint8_t key);
    virtual void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");

  Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  //Serial.print("DN ");
  //PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  if(key == 176) {
    Serial.println("Enter pressed");
  }
  
  Serial.print("ASCII: ");
  Serial.println((char)key);
};

USB     Usb;
USBHub     Hub1(&Usb);
//USBHub     Hub2(&Usb);
//USBHub     Hub3(&Usb);
//USBHub     Hub4(&Usb);
//USBHub     Hub5(&Usb);
//USBHub     Hub6(&Usb);
HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard1(&Usb);
//HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard2(&Usb);
//HIDBoot<HID_PROTOCOL_KEYBOARD>    HidKeyboard3(&Usb);

KbdRptParser Prs;

void setup()
{
  Serial.begin( 115200 );
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  Serial.println("Starting...");

  if (Usb.Init() == -1)
    Serial.println("OSC did not start.");

  //delay(200);

  HidKeyboard1.SetReportParser(0, (HIDReportParser*)&Prs);
}

void loop()
{
  Usb.Task();
}

