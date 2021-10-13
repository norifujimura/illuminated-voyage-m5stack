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


//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
//const char * udpAddress = "172.20.10.1";
const char * udpAddress = "192.168.86.239";
const int udpPort = 4444;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

char packetBuffer[255]; //buffer to hold incoming packet

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
  connectToWiFi(networkName, networkPswd);
  
  xTaskCreatePinnedToCore(taskZero, "taskZero", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskOne, "taskOne", 4096, NULL, 1, NULL, 1);
  

}

void taskZero(void* arg) {
  int cnt = 0;
  while (1) {
    //printf("task thread_cnt=%ld\n", cnt++);
    //showId();
    send();
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
  showBatteryLevel();
}

void send(){
  String buf = "botton_A";
  
  if(connected){
        //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    /*
    udp.printf("Seconds since boot: %lu", millis()/1000);
    */
        int i = 0;
    while( buf.length() > i){
      char temp = buf.charAt(i);
      udp.write(temp);
      i++;
    }
    udp.endPacket();
    Serial.print("Sent");
  }
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

      //send();
      /*
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write(ReplyBuffer);
      udp.endPacket();
      */
    }
  }
}

void connectToWiFi(const char * ssid, const char * pwd){
  ip = "";
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          ip = WiFi.localIP().toString();
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          connectToWiFi(networkName, networkPswd);
          break;
      default: break;
    }
}
