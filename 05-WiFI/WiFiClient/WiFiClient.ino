/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <M5Core2.h>
#include <WiFi.h>

/*
const char* ssid     = "voyager";
const char* password = "fujimura";
const char* host = "172.20.10.1";
*/

const char* ssid     = "akiko_network";
const char* password = "akobagus";
const char* host = "192.168.86.26";

// Use WiFiClient class to create TCP connections
WiFiClient client;
const int httpPort = 3333;

String id = "";

void setup()
{
    M5.begin(); 
    M5.Lcd.setTextSize(2);
    setupId();

    //Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    M5.lcd.print("Connecting to ");
    M5.lcd.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        M5.lcd.print(".");
    }

    M5.lcd.println("");
    M5.lcd.println("WiFi connected");
    M5.lcd.println("IP address: ");
    M5.lcd.println(WiFi.localIP());
}

int value = 0;

bool connect(){
    M5.lcd.print("connecting to ");
    M5.lcd.println(host);
    if (!client.connect(host, httpPort)) {
        M5.lcd.println("connection failed");
        return false;
    }else{
      M5.lcd.println("connected");
        return true;
    }
}

void setupId(){
  unsigned int auiSize = 0;
  unsigned int auiCnt = 0;
  
  File f = SD.open("/id.txt");
  if (f) {
    M5.Lcd.println("File open successful");
    // ファイルサイズ取得
    auiSize = f.size();
    // サイズ分ループ
    for( auiCnt = 0; auiCnt < auiSize; auiCnt++ )
    {
      
      // ファイルの中身を表示
      f.seek(auiCnt);
 
      id += (char)f.read();
      //M5.Lcd.printf("%c",f.read());
      
    }
    // ファイルクローズ   
    f.close();
    M5.Lcd.println("ID:"+id);
  } else {
    M5.Lcd.println("File open error hello.txt");
  }
}

void loop()
{
    if (!client.connected()) {
      connect();
    }
    
    delay(1);
    ++value;

    String url = "M5Test:"+String(value);

    Serial.print("Requesting URL: ");
    Serial.println(url);

    /*
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    */
    //client.print(url);
    
    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            //client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print("Received:"+line);
    }
}
