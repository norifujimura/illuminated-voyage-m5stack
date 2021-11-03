//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#include <M5Core2.h>
BluetoothSerial SerialBT;

void setup() {
  M5.begin();
  //Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  xTaskCreatePinnedToCore(task, "Task", 4096, NULL, 1, NULL, 1);
}

void task(void* arg) {
  int cnt = 0;
  while (1) {
    printf("task thread_cnt=%ld\n", cnt++);
    updateBatteryLevel();
    delay(1000);
  }
}

void loop() {
  showBatteryLevel();
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n');
    Serial.write(s.c_str());
  }
  if (SerialBT.available()) {
    String s = SerialBT.readStringUntil('\n');
    M5.Lcd.setCursor(0, 240-100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print(s);
    //SerialBT.write("R");
    SerialBT.println("Received:"+s);
  }
  delay(1);
}
