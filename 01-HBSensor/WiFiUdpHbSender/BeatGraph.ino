//HB


unsigned long beatNow = 0;
unsigned long beatLast = 0;


//int beatGraphWidth = M5.Lcd.width();
int width = 106;//320 /3
int height = M5.Lcd.height();

int beats[106];
int rates[106];
int oTwos[106];

void setupBeatGraph(){
   for(int i = 0 ; i< width; i++){
    beats[i] = 0;
    rates[i] = 0;
    oTwos[i] = 0;
  }
}

void updateBeatGraph(int value){

  if(value>0){
      beatLast = beatNow;
    beatNow = millis();
    beatDiff = beatNow- beatLast;
  }else{
    beatDiff = 0;
  }

  
  float ratef = pox.getHeartRate();
  int ratei = (int) ratef;
  if(ratei > 160){
    ratei = 160;
  }
  
  int oTwo = pox.getSpO2() ;//percent, up to 255?
  if(oTwo > 160){
    oTwo = 160;
  }

  /*
    Serial.print("Heart rate:");
    Serial.print(ratef);
    Serial.print("bpm / SpO2:");
    Serial.print(oTwo);
    Serial.println("%");
    */
      
  //Shift
  for(int i = (width-1) ; i > -1 ; i--){
    beats[i+1] = beats[i];
  }
  for(int i = (width-1) ; i > -1 ; i--){
    rates[i+1] = rates[i];
  }
  for(int i = (width-1) ; i > -1 ; i--){
    oTwos[i+1] = oTwos[i];
  }
  
  beats[0] = value;
  rates[0] = ratei;
  oTwos[0] = oTwo;
}

void showBeatGraph(){
  for(int i = 0 ; i< width; i++){
    M5.Lcd.drawFastVLine(i*3, 240-80-oTwos[i], oTwos[i], DARKGREY);  
    M5.Lcd.drawFastVLine(i*3+1, 240-80-rates[i], rates[i], LIGHTGREY);  
    M5.Lcd.drawFastVLine(i*3+2, 240-80-beats[i], beats[i], WHITE);  
  }
}
