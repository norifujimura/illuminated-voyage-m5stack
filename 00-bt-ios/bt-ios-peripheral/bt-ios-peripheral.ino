#include <M5Core2.h>

///////////////
//bool updateColor;
//String lastColor;
int num = 0;
TouchPoint_t touch;

//FPS
unsigned long now = 0;
unsigned long last = 0;

//BT
//#include <BLEDevice.h>
//bool deviceConnected = false;
//BLECharacteristic * pNotifyCharacteristic;

#define LOCAL_NAME                  "M5Stack"
//BT-FPS
unsigned long btNow = 0;
unsigned long btLast = 0;

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("Setup....");
  
  initBLE();
  //initLCDcolor();
  
  M5.Lcd.println("Done");
  if(last == 0){
    last = millis();
  }
}

// the loop routine runs over and over again forever
void loop() {
  M5.update();
  //M5.Lcd.clear(); 
  //M5.Lcd.fillScreen(BLACK);  
  showFps();
  loopBLE();

  //loopLCDcolor();
  checkBleMessage();
  showBatteryLevel();
  showBtFps();
  showLines();

  if(M5.Touch.ispressed()) {
    //Serial.println("Pressed");
    touch = M5.Touch.getPressPoint();
    int x =  255.0 /320.0 * touch.x;
    sendBleMessage(String(x));
  }
}

void showFps(){
  last = now;
  now = millis();
  int diff = now - last;
  float fps = 1000.0 / (float) diff;
  int ifps = (int)fps;
  String s = String("FPS:")+String(ifps).c_str()+String("fps  ");
  M5.Lcd.setCursor(0, 240-60);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}

void showBtFps(){
  int diff = btNow - btLast;
  float fps = 1000.0 / (float) diff;
  int ifps = (int)fps;
  String s = String("BT FPS:")+String(ifps).c_str()+String("fps                       ");
  M5.Lcd.setCursor(0, 240-90);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}

void showBatteryLevel(){
  //https://community.m5stack.com/topic/2994/core2-how-to-know-the-current-battery-level/4
  float batVoltage = M5.Axp.GetBatVoltage();
  float batPercentage = ( batVoltage < 3.2 ) ? 0 : ( batVoltage - 3.2 ) * 100;
  String s = String("Battery:")+String(batPercentage ).c_str()+String("%");
   //M5.Lcd.drawString(s, 0, 100, 1); 
  int i = (int) batPercentage;
  M5.Lcd.setCursor(0, 240-30);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
  M5.Lcd.progressBar(0, 240-10, 320, 20, i); 
}

void showLines(){
  int x = 320.0 / 255.0 * num;
  M5.Lcd.fillRect(0, 20, 320, 100, BLACK);
  M5.Lcd.drawFastVLine(x, 20, 100,LIGHTGREY); 
  M5.Lcd.drawFastVLine(touch.x, 20, 100,DARKGREY); 
}

void checkBleMessage(){

  //r
  String cmd = getBleMessage();
  if(cmd != ""){
    btLast = btNow;
    btNow = millis();
    M5.Lcd.setCursor(0, 240-120);
    M5.Lcd.setTextSize(3);
    num = cmd.toInt();
    M5.Lcd.print(cmd+"  :"+num+"  ");
  }


  //t
  
  if (isBleDeviceConnected()) {
    if (M5.BtnA.wasPressed()){
      sendBleMessage("A");
    }
    if (M5.BtnB.wasPressed()){
      sendBleMessage("B");
    }
    if (M5.BtnC.wasPressed()){
      sendBleMessage("C");
    }
  }

  /*
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
  */
}

/*
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
*/
