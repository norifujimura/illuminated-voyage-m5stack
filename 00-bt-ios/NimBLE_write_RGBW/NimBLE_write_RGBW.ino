
#include <M5Core2.h>
#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


//LED
#include <Adafruit_NeoPixel.h>
#define LED_PIN   32
#define LED_COUNT 150
#define BRIGHTNESS 64// Set BRIGHTNESS to about 1/4 (max = 255)
#define id "01"
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);
//

//App
unsigned long now = 0;
unsigned long last = 0;
String myValue;
String raw[10] = {"\0"};// H,S,I for 50 pixels
int state = 0;
//

void process(){
    Serial.println("Process:"+myValue);

    int l = split(myValue, ',',raw);

    Serial.println("State:"+raw[0]);
    state = raw[0].toInt(); 
}


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      /*
      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
      }
      */
      if (value.length() > 0) {
        showFps();
        String s = value.c_str();
        myValue = s;
        process();
      }
    }

    void showFps(){
      last = now;
      now = millis();
      int diff = now - last;
      float fps = 1000.0 / (float) diff;
      int ifps = (int)fps;
      String s = String("BTFPS:")+String(ifps).c_str()+String("fps  ");
      M5.Lcd.setCursor(0, 240-60);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print(s);
    }
};

void btSetup() {
  Serial.println("btSetUp");
  BLEDevice::init(id);

  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                  /***** Enum Type NIMBLE_PROPERTY now *****      
                                        BLECharacteristic::PROPERTY_READ   |
                                        BLECharacteristic::PROPERTY_WRITE  
                                        );
                                  *****************************************/
                                        NIMBLE_PROPERTY::READ |
                                        NIMBLE_PROPERTY::WRITE 
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void ledSetup(){
  Serial.println("ledSetup");        
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
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

void showFps(){
  last = now;
  now = millis();
  int diff = now - last;
  float fps = 1000.0 / (float) diff;
  int ifps = (int)fps;
  String s = String("LoopFPS:")+String(ifps).c_str()+String("fps  ");
  M5.Lcd.setCursor(0, 240-80);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}

void showState(){
  String s = String("STATE:")+String(state)+"   ";
  M5.Lcd.setCursor(0, 240-100);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}

void showId(){
  String s = id;
  M5.Lcd.setCursor(0, 240-120);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}

//https://algorithm.joho.info/arduino/string-split-delimiter/

int split(String data, char delimiter, String *dst){

    int index = 0;
    int arraySize = (sizeof(data)/sizeof((data)[0]));  
    int datalength = data.length();

    for (int i = 0; i < 10; i++) {
      dst[i] = "";
    }
    
    for (int i = 0; i < datalength; i++) {
        char tmp = data.charAt(i);
        if ( tmp == delimiter ) {
            index++;
            if ( index > (arraySize - 1)) return -1;
        }
        else dst[index] += tmp;
        //else dst[index] = tmp;
    }
    return (index + 1);
    //return dst;
}

void setup() {
  M5.begin();
  //Serial.begin(115200);
  Serial.println("setup0");
  M5.Lcd.fillScreen(BLACK);
  Serial.println("setup1");
  btSetup();
  ledSetup();
  showId();
  showBatteryLevel();
}

void loop() {
  //Serial.println("loop0");
  //M5.update();
  //Serial.println("loop1");
  showFps();
  showState();
  showBatteryLevel();
  delay(150);//100ms is not enough to make a room for callback
}
