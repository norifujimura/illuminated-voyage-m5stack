

/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

/** NimBLE differences highlighted in comment blocks **/

/*******original********
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
***********************/

#include <M5Core2.h>
#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

unsigned long now = 0;
unsigned long last = 0;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
      showFps();
    }

    void showFps(){
      last = now;
      now = millis();
      int diff = now - last;
      float fps = 1000.0 / (float) diff;
      int ifps = (int)fps;
      String s = String("BTFPS:")+String(ifps).c_str()+String("fps  ");
      M5.Lcd.setCursor(0, 240-60);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print(s);
    }
};

void setup() {
  M5.begin();
  //Serial.begin(115200);
  /*
  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");
  */

  xTaskCreatePinnedToCore(task, "Task", 4096, NULL, 1, NULL, 1);

  BLEDevice::init("01");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                  /***** Enum Type NIMBLE_PROPERTY now *****      
                                        BLECharacteristic::PROPERTY_READ   |
                                        BLECharacteristic::PROPERTY_WRITE  
                                        );
                                  *****************************************/
                                        /*NIMBLE_PROPERTY::READ |*/
                                        NIMBLE_PROPERTY::WRITE 
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void task(void* arg) {
  int cnt = 0;
  while (1) {
    printf("task thread_cnt=%ld\n", cnt++);
    delay(200);
  }
}

void loop() {
  //showBatteryLevel();
  showFps();
  delay(300);//around 10fps
}

void showBatteryLevel(){
  if((now % 1000) < 100){
     Serial.println("showBettryLevel");
  }else{
    return;
  }
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

void showFps(){
  last = now;
  now = millis();
  int diff = now - last;
  float fps = 1000.0 / (float) diff;
  int ifps = (int)fps;
  String s = String("LoopFPS:")+String(ifps).c_str()+String("fps  ")+String(now);
  M5.Lcd.setCursor(0, 240-80);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}
