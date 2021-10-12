

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

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     100

PulseOximeter pox;
uint32_t tsLastReport = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

unsigned long now = 0;
unsigned long last = 0;

String id = "";
int batteryCounter = 0;
int batteryCounterLimit = 10000;

void setup() {
  M5.begin();
  setupId();
  //Serial.begin(115200);
  //xTaskCreatePinnedToCore(task, "Task", 4096, NULL, 1, NULL, 1);
  setupBT();
  setupHB();
}

void task(void* arg) {
  int cnt = 0;
  while (1) {
    printf("task thread_cnt=%ld\n", cnt++);
    delay(200);
  }
}

void loop() {

  
  if(batteryCounter>batteryCounterLimit){
    batteryCounter = 0;
    showBatteryLevel();
  }
  batteryCounter++;
  

  // Make sure to call update as fast as possible
  pox.update();
  
  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.print("bpm / SpO2:");
    Serial.print(pox.getSpO2());
    Serial.println("%");
  
    tsLastReport = millis();
  }
  
  showId();
  showFps();

  //notify();

  //delay(1000);//around 10fps
}
