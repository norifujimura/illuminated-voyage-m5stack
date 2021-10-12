
bool isUpdated = false;
BLECharacteristic *pCharacteristic;
/*
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
          Serial.print(value[i]);

        Serial.println();
        Serial.println("*********");
        String s = value.c_str();
        myValue = s;
        process();
      }
      showFps();
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
*/
/*
void process(){
    Serial.println("Process:"+myValue);
    int l = split(myValue, ',',raw);

    Serial.println("State:"+raw[0]);
    state = raw[0].toInt(); 
    isUpdated = true;
}
*/

void notify(){
      //if (deviceConnected) {
        //pCharacteristic->setValue((uint8_t*)&value, 4);
        pCharacteristic->setValue("BEAT");
        pCharacteristic->notify();
        //value++;
        //delay(3); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    //}
}

void setupBT(){
  
  BLEDevice::init(id.c_str());
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  /*
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
  
                                        NIMBLE_PROPERTY::NOTIFY 
                                       );
                                       */
   pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
  
                                        NIMBLE_PROPERTY::NOTIFY 
                                       );

  //pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}
