

//The udp library class



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
          udp.begin(WiFi.localIP(),port);
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
