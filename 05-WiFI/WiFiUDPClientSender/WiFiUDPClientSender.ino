/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */

#include <M5Core2.h>

#include <WiFi.h>
#include <WiFiUdp.h>


// WiFi network name and password:

const char * networkName = "akiko_network";
const char * networkPswd = "akobagus";


/*
const char * networkName = "voyager";
const char * networkPswd = "fujimura";
*/

//App
String id = "";
String ip = "";
unsigned long now = 0;
unsigned long last = 0;


void setup(){
  // Initilize hardware serial:
  //Serial.begin(115200);
  
  M5.begin();
  M5.Lcd.clear(); 
  setupId();
    //Connect to the WiFi network
  setupWiFi(networkName, networkPswd);
  
  xTaskCreatePinnedToCore(taskZero, "taskZero", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskOne, "taskOne", 4096, NULL, 1, NULL, 1);
  

}

void taskZero(void* arg) {
  int cnt = 0;
  while (1) {
    //printf("task thread_cnt=%ld\n", cnt++);
    //showId();
    send();
    
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

void loop(){

  
  //Wait for 1 second
  delay(100);
  
  showFps();
  showId();
  showIp();
  showBatteryLevel();
}
