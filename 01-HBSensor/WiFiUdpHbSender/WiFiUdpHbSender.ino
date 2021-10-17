/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */

#include <M5Core2.h>

#include <WiFi.h>
#include <WiFiUdp.h>

//#include <Array.h>

// WiFi network name and password:

//
//
/*
const char * udpAddress = "192.168.86.239";
const char * networkName = "akiko_network";
const char * networkPswd = "akobagus";
*/


const char * udpAddress = "172.20.10.1";
const char * networkName = "voyager";
const char * networkPswd = "fujimura";



//HbSensor
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define REPORTING_PERIOD_MS     100
PulseOximeter pox;
uint32_t tsLastReport = 0;

//App
String id = "";
String ip = "";
unsigned long now = 0;
unsigned long last = 0;

//bool isBtnPressed = false;

int beatDiff = 0;

void setup(){
  // Initilize hardware serial:
  //Serial.begin(115200);
  
  M5.begin();
  M5.Lcd.clear(); 
  setupId();
  
  int port = 9999;
  if(id == "04"){
    port = 4444;
  }
  if(id == "05"){
    port = 5555;
  }

  Serial.println("PORT set: " + String(port));
  
  setupHB();
    //Connect to the WiFi network
  setupWiFi(networkName, networkPswd,port);
  setupBeatGraph();
  
  xTaskCreatePinnedToCore(taskZero, "taskZero", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskOne, "taskOne", 4096, NULL, 1, NULL, 1);
}

void taskZero(void* arg) {
  int cnt = 0;
  while (1) {
    //printf("task thread_cnt=%ld\n", cnt++);
    //showId();
    //send();
    delay(50);
  }
}

void taskOne(void* arg) {
  while (1) {
    //read();
    updateBatteryLevel();
    delay(1000);
  }
}

void beat(){
  updateBeatGraph(160);
  send(String(beatDiff));
}

void loop(){

  M5.update();    //You need to add M5.update () to read the status of the keystroke. For details, please see System. 
  // Make sure to call update as fast as possible
  pox.update();
  
  //M5.Lcd.clear();
  M5.Lcd.fillRect(0,0, 320, 180, BLACK);  
  M5.Lcd.setCursor(0, 240-60);

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    updateBeatGraph(0);
    tsLastReport = millis();
  }

    /*
    if (M5.BtnA.isPressed()) {
      //M5.Speaker.tone(661); //Set the speaker to tone continuously at 661Hz
      M5.Lcd.println("Button is pressed.");
      isBtnPressed = true;
    }
    
    if (isBtnPressed && M5.BtnA.isReleased()) {    //If the key is pressed. 
      //M5.Speaker.end(); //Turn off the speaker
      M5.Lcd.println("Button is released.");
      isBtnPressed = false;

      updateBeatGraph(160);

      send(String(beatDiff));
      
    }else{
      M5.Lcd.println("Button is free");
      updateBeatGraph(0);
    }
    */
  
  //Wait for 1 second
  delay(1);
  showBeatGraph();
  showFps();
  showId();
  showIp();
  showPort();
  showSide();
  showBatteryLevel();
}
