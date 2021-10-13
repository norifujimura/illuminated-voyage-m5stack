/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */
#include <WiFi.h>
#include <WiFiUdp.h>

// WiFi network name and password:
const char * networkName = "akiko_network";
const char * networkPswd = "akobagus";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "192.168.86.239";
const int udpPort = 3333;

char ReplyBuffer[] = "acknowledged";   

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

void setup(){
  // Initilize hardware serial:
  Serial.begin(115200);
  
  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
}

void loop(){
  //only send data when connected
  if(connected){
    //byte packetBuffer[255];
    //strcpy(packetBuffer,"This is a test");

    String buf = "botton_A";
    
    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    int i = 0;
    while( buf.length() > i){
      char temp = buf.charAt(i);
      udp.write(temp);
      i++;
    }
    udp.endPacket();
    Serial.println("Sent");
  }
  //Wait for 1 second
  delay(1000);
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
          break;
      default: break;
    }
}
