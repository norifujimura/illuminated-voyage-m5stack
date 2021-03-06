
bool isUpdated = false;

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

void process(){
    Serial.println("Process:"+myValue);
    int l = split(myValue, ',',raw);

    Serial.println("State:"+raw[0]);
    if(state == 16){
      
    }else{
      lastState = state;
    }
    state = raw[0].toInt(); 
    isUpdated = true;
}

void setupBT(){
  
  BLEDevice::init(id.c_str());
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID,
                                  /***** Enum Type NIMBLE_PROPERTY now *****      
                                        BLECharacteristic::PROPERTY_READ   |
                                        BLECharacteristic::PROPERTY_WRITE  
                                        );
                                  *****************************************/
                                        /*NIMBLE_PROPERTY::READ |*/
                                        NIMBLE_PROPERTY::WRITE 
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}
