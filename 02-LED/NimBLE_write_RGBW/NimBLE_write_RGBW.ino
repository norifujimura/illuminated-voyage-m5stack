

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

String id = "03";
String myValue;
String raw[10] = {"\0"};// H,S,I for 50 pixels

int state = 0;
int lastState = 0;
int batteryCounter = 0;
int batteryCounterLimit = 10000;

void setup() {
  M5.begin();
  //setupId();
  //Serial.begin(115200);
  xTaskCreatePinnedToCore(task, "Task", 4096, NULL, 1, NULL, 1);
  setupBT();
  setupLED();
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

  showId();
  showFps();
  showState();
  showBatteryLevel();

  if(state == 0){
    colorWipeTwo(200,50,50,0,50);
  }
  if(state == 1){
    colorWipeTwo(200,50,50,0,15);
  }
  
  if(state == 2){
    colorWipeTwo(170,100,100,0,50);
  }
  if(state == 3){
    colorWipeTwo(170,100,100,0,15);
  }

  if(state == 4){
    colorWipeTwo(200,50,200,0,50);
  }
  if(state == 5){
    colorWipeTwo(200,50,200,0,15);
  }

  if(state == 6){
    colorWipeTwo(50,50,200,0,50);
  }
  if(state == 7){
    colorWipeTwo(50,50,200,0,15);
  }

  if(state == 8){
    colorWipeTwo(50,100,200,0,50);
  }
  if(state == 9){
    colorWipeTwo(50,100,200,0,15);
  }

  if(state == 10){
    colorWipeTwo(100,100,100,255,50);
  }
  if(state == 11){
    colorWipeTwo(100,100,100,255,15);
  }
  
  if(state == 12){
    whiteOverRainbow(75, 5);
  }
  if(state == 13){
    pulseWhite(5);
  }
  if(state == 14){
    rainbowFade2White(3, 3, 1);
  }
  if(state == 15){
    pulseWhiteForIdle(10);
  }
  if(state == 16){
    pulse(1);
  }
  if(state == 17){
    pulseBase(20);
  }
  if(state == 99){
    
  }
  
  delay(150);//around 10fps
}
