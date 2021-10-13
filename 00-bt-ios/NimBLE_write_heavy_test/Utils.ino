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

float batteryPercentage;

void updateBatteryLevel(){
    //https://community.m5stack.com/topic/2994/core2-how-to-know-the-current-battery-level/4
  float batteryVoltage = M5.Axp.GetBatVoltage();
  batteryPercentage = ( batteryVoltage < 3.2 ) ? 0 : ( batteryVoltage - 3.2 ) * 100;
  
   //M5.Lcd.drawString(s, 0, 100, 1); 
}

void showBatteryLevel(){
  int i = (int) batteryPercentage;
  String s = String("Battery:")+String(batteryPercentage ).c_str()+String("%");
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
  String s = String("LoopFPS:")+String(ifps).c_str()+String("fps  ")+String(now);
  M5.Lcd.setCursor(0, 240-80);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}

void showId(){

  String s = String("ID:")+id;
  M5.Lcd.setCursor(0, 240-120);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print(s);
}

void showState(){
  String s = String("STATE:")+String(state)+"   ";
  M5.Lcd.setCursor(0, 240-100);
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
