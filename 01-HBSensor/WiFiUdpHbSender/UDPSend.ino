
//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address

//const int udpPort = 4444;
int udpPort = 9999;


//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

void setupWiFi(const char * ssid, const char * pwd, int port){
  ip = "";
  udpPort = port;
  
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
          setupWiFi(networkName, networkPswd, udpPort);
          break;
      default: break;
    }
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
    Serial.print("S");
  }
}

void send(String t){
  String buf = t;
  
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
    Serial.print("Sent:"+ t + ",");
  }
}



void read(){
    if(connected){
    char packetBuffer[255];
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
