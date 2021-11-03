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
//https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/src/BluetoothSerial.h

//BTSerial RX side is 512 by default
//https://www.esp32.com/viewtopic.php?t=22132
//

unsigned long serialNow = 0;
unsigned long serialLast = 0;

String serialArray[100];

void setup() {
  M5.begin();
  //Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.setRxBufferSize(1024);
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
  showFps();
  /*
  if (Serial.available()) {String
    String s = Serial.readStringUntil('\n');
    Serial.write(s.c_str());
  }
  */
  if (SerialBT.available()) {
    String s = SerialBT.readStringUntil('\n');
    

    int index = split(s,',',serialArray,100);

    if(index == (-1)){
      return;
    }

    
    if(index < 100){
      return;
    }
    

    showSerialFps();
    
    M5.Lcd.setCursor(0, 240-100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print(String(index) + ":"+serialArray[99]+"  ");

    uint16_t colorvalue = 0;
    for(int i = 0; i<100; i++){
        int col = serialArray[i].toInt();
        colorvalue = M5.Lcd.color565(col,col,col);
        M5.Lcd.fillRect(i*3, 0, 3, 100, colorvalue );   
    }
 
    //SerialBT.write("R");
    //SerialBT.println("Received:"+s);
  }
  delay(1);
}

void showSerialFps(){
  serialLast = serialNow;
  serialNow = millis();
  int diff = serialNow - serialLast;
  float fps = 1000.0 / (float) diff;
  int ifps = (int)fps;
  String s = String("SerialFPS:")+String(ifps).c_str()+String("fps  ")+String(serialNow);
  M5.Lcd.setCursor(0, 240-60);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}
