/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */

#include <M5Core2.h>

#include <WiFi.h>
#include <WiFiUdp.h>

//#include <Array.h>

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

//HB
bool isBtnPressed = false;

unsigned long beatNow = 0;
unsigned long beatLast = 0;

//int beatGraphWidth = M5.Lcd.width();
int beatGraphWidth = 320;
int beatGraphHeight = M5.Lcd.height();

//Array<int,320> beatGraphArray;
int beatGraphArray[320];
int beatGraphCursor = 0;

void setup(){
  // Initilize hardware serial:
  //Serial.begin(115200);
  
  M5.begin();
  M5.Lcd.clear(); 
  setupId();
    //Connect to the WiFi network
  setupWiFi(networkName, networkPswd);
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

void loop(){

    M5.update();    //You need to add M5.update () to read the status of the keystroke. For details, please see System. 

    //M5.Lcd.clear();
    M5.Lcd.fillRect(0,0, 320, 180, BLACK);  
    M5.Lcd.setCursor(0, 240-60);
    
    if (M5.BtnA.isPressed()) {
      //M5.Speaker.tone(661); //Set the speaker to tone continuously at 661Hz
      M5.Lcd.println("Button is pressed.");
      isBtnPressed = true;
      
    }
    
    if (isBtnPressed && M5.BtnA.isReleased()) {    //If the key is pressed. 
      //M5.Speaker.end(); //Turn off the speaker
      M5.Lcd.println("Button is released.");
      isBtnPressed = false;
        
      beatLast = beatNow;
      beatNow = millis();
      int diff = beatNow - beatLast;
      send(String(diff));
      updateBeatGraph(100);
    }else{
      M5.Lcd.println("Button is free");
      updateBeatGraph(0);
    }
  
  //Wait for 1 second
  delay(1);
  showBeatGraph();
  showFps();
  showId();
  showIp();
  showBatteryLevel();
}

void setupBeatGraph(){
   for(int i = 0 ; i< 320; i++){
    beatGraphArray[i] = 0;
  }
}

void updateBeatGraph(int value){
  //Shift
  for(int i = 319 ; i > -1 ; i--){
    beatGraphArray[i+1] = beatGraphArray[i];
  }
  
  beatGraphArray[0] = value;
}

void showBeatGraph(){
  for(int i = 0 ; i< 320; i++){
    M5.Lcd.drawFastVLine(i, 240-80-beatGraphArray[i], beatGraphArray[i], LIGHTGREY);  
  }
}
