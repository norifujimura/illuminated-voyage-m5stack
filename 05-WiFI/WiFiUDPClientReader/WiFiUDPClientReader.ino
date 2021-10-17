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
int port = 9999;

//Are we currently connected?
boolean connected = false;
WiFiUDP udp;
char packetBuffer[1024]; //buffer to hold incoming packet

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
  setupPort();
  
    //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
  
  xTaskCreatePinnedToCore(taskZero, "taskZero", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskOne, "taskOne", 4096, NULL, 1, NULL, 1);
}

void taskZero(void* arg) {
  int cnt = 0;
  while (1) {
    //printf("task thread_cnt=%ld\n", cnt++);
    //showId();
    //send();
    updateBatteryLevel();
    delay(1000);
  }
}

void taskOne(void* arg) {
  while (1) {
    read();
    delay(10);
  }
}

void loop(){

  //Wait for 1 second
  delay(100);
  
  showFps();
  showId();
  showIp();
  showPort();
  showBatteryLevel();
}

void read(){
    if(connected){
    
    int packetSize = udp.parsePacket();
    if (packetSize) {
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remoteIp = udp.remoteIP();
      Serial.print(remoteIp);
      Serial.print(", port ");
      Serial.println(udp.remotePort());

      int len = udp.read(packetBuffer, 255);
      if (len > 0) {
        packetBuffer[len] = 0;
      }

      //udp.flush();

      Serial.println("Contents:");
      Serial.println(packetBuffer);
    }
  }
}
