#include <M5Core2.h>

///////////////
bool updateColor;
String lastColor;

//FPS
unsigned long now = 0;
unsigned long last = 0;

//BT
//#include <BLEDevice.h>
//bool deviceConnected = false;
//BLECharacteristic * pNotifyCharacteristic;

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();
  M5.Lcd.setTextSize(1);
  M5.Lcd.print("Setup....");
  
  initBLE();
  initLCDcolor();
  
  M5.Lcd.println("Done");
  if(last == 0){
    last = millis();
  }
}

// the loop routine runs over and over again forever
void loop() {
  M5.update();
  showFps();
  loopBLE();
  loopBLE();
  loopLCDcolor();
  checkBleMessage();
  showBatteryLevel();
}

void showFps(){
  now = millis();
  int diff = now - last;
  float fps = 1000.0 / (float) diff;
  String s = String("FPS:")+String(fps).c_str();
  M5.Lcd.setCursor(0, 240-40);
  M5.Lcd.print(s);
  last = now;
}

void showBatteryLevel(){
  //https://community.m5stack.com/topic/2994/core2-how-to-know-the-current-battery-level/4
  float batVoltage = M5.Axp.GetBatVoltage();
  float batPercentage = ( batVoltage < 3.2 ) ? 0 : ( batVoltage - 3.2 ) * 100;
  String s = String("Battery:")+String(batPercentage ).c_str()+String("%");
   //M5.Lcd.drawString(s, 0, 100, 1); 
  int i = (int) batPercentage;
  M5.Lcd.setCursor(0, 240-20);
  M5.Lcd.print(s);
  M5.Lcd.progressBar(0, 240-10, 320, 20, i); 
}

void checkBleMessage(){
  String cmd = getBleMessage();
  if (cmd == "RED"){
    // RED
    lastColor = "RED";
    updateColor = true;
  }
  if (cmd == "YELLOW"){
    // YELLOW
    lastColor = "YELLOW";
    updateColor = true;
  }
  if (cmd == "BLUE"){
    // BLUE
    lastColor = "BLUE";
    updateColor = true;
  }
}

///////////////
// LCD Color //
///////////////
void initLCDcolor() {
  lastColor = "NONE";
}

void loopLCDcolor() {
  if (M5.BtnA.wasPressed())
  {
    lastColor = "RED";
    updateColor = true;
  }
  if (M5.BtnB.wasPressed())
  {
    lastColor = "YELLOW";
    updateColor = true;
  }
  if (M5.BtnC.wasPressed())
  {
    lastColor = "BLUE";
    updateColor = true;
  }
  
  if (updateColor) {
    if (lastColor == "RED")
    {
      // RED
      M5.Lcd.fillScreen(RED);
    }
    if (lastColor == "YELLOW")
    {
      // YELLOW
      M5.Lcd.fillScreen(YELLOW);
    }
    if (lastColor == "BLUE")
    {
      // BLUE
      M5.Lcd.fillScreen(BLUE);
    }
    if (isBleDeviceConnected()) {
      sendBleMessage(lastColor);
    }
    updateColor = false;
  }
}
