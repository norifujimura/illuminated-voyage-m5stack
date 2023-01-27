#include <M5Core2.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  SerialBT.begin("ESP32");
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
}

void showBatteryLevel(){
  //https://community.m5stack.com/topic/2994/core2-how-to-know-the-current-battery-level/4
  float batVoltage = M5.Axp.GetBatVoltage();
  float batPercentage = ( batVoltage < 3.2 ) ? 0 : ( batVoltage - 3.2 ) * 100;
  String s = String("Battery:")+String(batPercentage ).c_str()+String("%");
   //M5.Lcd.drawString(s, 0, 100, 1); 
  int i = (int) batPercentage;
  M5.Lcd.setCursor(0, 240-30);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
  M5.Lcd.progressBar(0, 240-10, 320, 20, i); 
}

void loop() {
   //M5.Lcd.fillScreen(BLACK);
  if (SerialBT.available()>0) {
    int av = SerialBT.available();
    int re = SerialBT.read();
    String sAv = String(av);
    String sRe = String(re);

    String rcv = ("RCV:"+sRe+" Available:"+sAv);
    
    M5.Lcd.setCursor(0, 240-90);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print(rcv);
      
    SerialBT.println("Response:"+rcv);
    Serial.write(SerialBT.read());
  }
  showBatteryLevel();
  SerialBT.println("Alive:");
  delay(10);
}
