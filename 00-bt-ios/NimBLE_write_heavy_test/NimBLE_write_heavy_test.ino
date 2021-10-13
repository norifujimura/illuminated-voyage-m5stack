

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

String id = "";
String myValue;
String raw[10] = {"\0"};// H,S,I for 50 pixels
int state = 0;

void setup() {
  M5.begin();
  M5.Lcd.clear(); 
  setupId();
  //Serial.begin(115200);
  xTaskCreatePinnedToCore(task, "Task", 4096, NULL, 1, NULL, 1);
  setupBT();
  
}

void task(void* arg) {
  int cnt = 0;
  while (1) {
    //printf("task thread_cnt=%ld\n", cnt++);
    //showId();
    updateBatteryLevel();
    delay(1000);
  }
}

void process(){
    Serial.println("Process:"+myValue);
    int l = split(myValue, ',',raw);
    Serial.println("State:"+raw[0]);
    state = raw[0].toInt(); 
}

void loop() {
  showId();
  showBatteryLevel();
  showFps();
  showState();
  delay(150);//around 10fps
}
