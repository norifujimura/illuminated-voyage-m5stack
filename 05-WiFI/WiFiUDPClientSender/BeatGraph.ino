//HB


unsigned long beatNow = 0;
unsigned long beatLast = 0;


//int beatGraphWidth = M5.Lcd.width();
int beatGraphWidth = 320;
int beatGraphHeight = M5.Lcd.height();
int beatGraphArray[320];

void setupBeatGraph(){
   for(int i = 0 ; i< 320; i++){
    beatGraphArray[i] = 0;
  }
}

void updateBeatGraph(int value){

  beatLast = beatNow;
  beatNow = millis();
  beatDiff = beatNow - beatLast;
      
  //Shift
  for(int i = 319 ; i > -1 ; i--){
    beatGraphArray[i+1] = beatGraphArray[i];
  }
  
  beatGraphArray[0] = value;
}

void showBeatGraph(){
  for(int i = 0 ; i< 320; i++){
    M5.Lcd.drawFastVLine(i, 240-80-beatGraphArray[i], beatGraphArray[i], LIGHTGREY);  
    //M5.Lcd.drawFastVLine(i, 0, 160, LIGHTGREY); 
    //M5.Lcd.drawFastVLine(i-1, 0, 240-80, LIGHTGREY); 
  }
}
