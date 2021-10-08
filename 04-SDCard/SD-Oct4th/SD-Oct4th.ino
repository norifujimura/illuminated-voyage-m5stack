
#include <M5Core2.h>

void setup() {
  // put your setup code here, to run once:
  unsigned int auiSize = 0;
  unsigned int auiCnt = 0;
  M5.begin();
  M5.Lcd.setTextSize(2);
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
      M5.Lcd.printf("%c",f.read());
    }
    // ファイルクローズ   
    f.close();
  } else {
    M5.Lcd.println("File open error hello.txt");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
