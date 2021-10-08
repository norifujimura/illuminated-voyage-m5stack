#include <M5StickC.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    32
#define LED_COUNT 300 // 60LEDs per 1m

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

bool isGo = true;
int counter = 0;
int limit = 1000;
int r = 250;
int g = 180;
int b = 20;

int locCounter = 0;
int locLimit = 16;
int brightness = 0; //up to 255

void setup() {
  brightness = 400 / locLimit;
  
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(YELLOW); 
  M5.Lcd.setTextSize(1); 
  M5.Lcd.setCursor(0,10);
  M5.Lcd.println("PIER LED");
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(0, 40);
  String s = String("Brightness:")+String(brightness).c_str();
  M5.Lcd.println(s);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.setBrightness(brightness);
  strip.show();            // Turn OFF all pixels ASAP
}



void loop() {

  if(locCounter>locLimit){
    locCounter = 0;
  }
  
  if(counter>limit){
    isGo = false;
    counter = limit;
  }else if(counter<0){
    isGo = true;
    counter = 0;

    locCounter++;
  }

  float ratio = (float)counter / (float)limit;
  float fr = (float)r * ratio;
  float fg = (float)g * ratio;
  float fb = (float)b * ratio;

  uint32_t color = strip.Color((int)fr,   (int)fg,   (int)fb);

  strip.clear();
  
  for(int i=0; i<strip.numPixels()/locLimit; i++) { 
    int loc = i*locLimit + locCounter;
    strip.setPixelColor(loc , color);
  }
  
  
  if(isGo){
    counter++;
  }else{
    counter--;
  }

  

  strip.show(); 
  
}
