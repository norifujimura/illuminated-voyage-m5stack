/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/
#include <M5Core2.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define CHARACTERISTIC_UUID_RX      "7a4a668e-0f71-11ec-82a8-0242ac130003"
#define CHARACTERISTIC_UUID_NOTIFY  "7a4a668e-0f71-11ec-82a8-0242ac130003"

#include <Adafruit_NeoPixel.h>
#define LED_PIN   32
//#define LED_COUNT 288
#define LED_COUNT 120//60 *2
//#define BRIGHTNESS 64// Set BRIGHTNESS to about 1/4 (max = 255)
#define BRIGHTNESS 200
#define id "03"
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

//#include "math.h"
//#define DEG_TO_RAD(X) (M_PI*(X)/180)
//#include "ColorConverter.h"
//https://github.com/tigoe/ColorConverter

unsigned long now = 0;
unsigned long last = 0;
//String id = "M5Stack-Front-L";
String myValue;
String raw[10] = {"\0"};// H,S,I for 50 pixels

int state = 0;
int counter = 0;
int counterLimit = 10000;
/*
int r = 127;
int g = 127;
int b = 127;
int w = 127;
int leds = 150;
int led_interval = 4;

int led_cycle = 10;
int led_cycle_counter = 0;

int led_speed = 1;
int led_speed_counter = 0;
int led_speed_counter_limit = 20;
bool isGo = true;
*/

void process(){
    Serial.println("Process:"+myValue);

    int l = split(myValue, ',',raw);

    Serial.println("State:"+raw[0]);
    state = raw[0].toInt();
    /*
    Serial.println("R:"+raw[0]);
    Serial.println("G:"+raw[1]);
    Serial.println("B:"+raw[2]);
    Serial.println("W:"+raw[3]);
    Serial.println("Interval:"+raw[4]);
    Serial.println("Cycle:"+raw[5]);
    Serial.println("Speed:"+raw[6]);

    r = raw[0].toInt();
    g = raw[1].toInt();
    b = raw[2].toInt();
    w = raw[3].toInt();
    led_interval = raw[4].toInt();
    led_cycle = raw[5].toInt();
    led_speed = raw[6].toInt();
    */
    
}

class MyCallbacks: public BLECharacteristicCallbacks {
      //FPS
      
    unsigned long now = 0;
    unsigned long last = 0;
    void onWrite(BLECharacteristic *pCharacteristic) {
      //Serial.println("OnWrite");
      std::string value = pCharacteristic->getValue();
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


void btSetup(){
  Serial.println("btSetUp");
   BLEDevice::init(id);
  //BLEDevice::setMTU(1080);
  BLEServer *pServer = BLEDevice::createServer();
  //pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic

  BLECharacteristic * pNotifyCharacteristic;
  pNotifyCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_NOTIFY,
                        BLECharacteristic::PROPERTY_NOTIFY
                        );
  
  pNotifyCharacteristic->addDescriptor(new BLE2902());


  
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         /*BLECharacteristic::PROPERTY_READ |*/
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
                                
 
  pCharacteristic->addDescriptor(new BLE2902());
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
  if(counter>counterLimit){
    counter = 0;
    showBatteryLevel();
  }
  //Serial.println("loop0");
  //M5.update();
  //Serial.println("loop1");
  showFps();
  showState();

  if(state == 0){
    colorWipeTwo(200,100,100,0,50);
  }
  if(state == 1){
    colorWipeTwo(200,100,100,0,15);
  }
  
  if(state == 2){
    colorWipeTwo(170,150,150,0,50);
  }
  if(state == 3){
    colorWipeTwo(170,150,150,0,15);
  }

  if(state == 4){
    colorWipeTwo(200,100,200,0,50);
  }
  if(state == 5){
    colorWipeTwo(200,100,200,0,15);
  }

  if(state == 6){
    colorWipeTwo(100,100,200,0,50);
  }
  if(state == 7){
    colorWipeTwo(100,100,200,0,15);
  }

  if(state == 8){
    colorWipeTwo(100,150,200,0,50);
  }
  if(state == 9){
    colorWipeTwo(100,150,200,0,15);
  }

  if(state == 10){
    colorWipeTwo(100,100,100,255,50);
  }
  if(state == 11){
    colorWipeTwo(100,100,100,255,15);
  }
  
  if(state == 12){
    whiteOverRainbow(75, 5);
  }
  if(state == 13){
    pulseWhite(5);
  }
  if(state == 14){
    rainbowFade2White(3, 3, 1);
  }
  if(state == 99){
    
  }
  
  delay(150);//100ms is not enough to make a room for callback
  counter++;
 
}

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void colorWipeTwo(int r,int g,int b,int w, int wait) {

  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
  //for(int i=0; i<LED_COUNT;i++){
    int counter = 0;
    for(int j=i; j>0; j--) {
      int rTwo = r-counter;
      if(rTwo<0){
        rTwo = 0;
      }
      int gTwo = g-counter;
      if(gTwo<0){
        gTwo = 0;
      }
      int bTwo = b-counter;
      if(bTwo<0){
        bTwo = 0;
      }
     int wTwo = w-counter;
      if(wTwo<0){
        wTwo = 0;
      }

      uint32_t  c = strip.Color(  rTwo,   gTwo,   bTwo, wTwo);
      strip.setPixelColor(j, c);         //  Set pixel's color (in RAM)
      //counter++;

      int step = 288/strip.numPixels();
      counter+=step;
    }

    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    delay(wait);

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
    delay(1000); // Pause 1 second
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
  }

  delay(500); // Pause 1/2 second
}
