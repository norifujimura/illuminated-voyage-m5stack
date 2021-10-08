/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */

#include <M5Core2.h>

#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
/*
const char * networkName = "akiko_network";
const char * networkPswd = "akobagus";
*/


const char * networkName = "pioneer";
const char * networkPswd = "akobagus";


//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "172.20.10.5";
//const char * udpAddress = "192.168.86.27";
const int udpPort = 80;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

char packetBuffer[255]; //buffer to hold incoming packet

void setup(){
  // Initilize hardware serial:
  //Serial.begin(115200);
  M5.begin();
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
}

void loop(){
  //only send data when connected
  if(connected){

    
    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    udp.printf("Seconds since boot: %lu", millis()/1000);
    //udp.write("Seconds since boot: %lu", millis()/1000);
    udp.endPacket();
    //Serial.print("Sent");
    
    
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

      Serial.println("Contents:");
      Serial.println(packetBuffer);
      /*
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write(ReplyBuffer);
      udp.endPacket();
      */
    }
  }
  
  //Wait for 1 second
  delay(50);
  showBatteryLevel();
}

void connectToWiFi(const char * ssid, const char * pwd){
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
