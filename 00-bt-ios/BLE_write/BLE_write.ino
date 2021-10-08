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


class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      //String value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        String msgOne = "Length: "+ String(value.length());
        Serial.print(msgOne);
        /*
        for (int i = 0; i < value.length(); i++){
          
        }
        */
        Serial.println();
        String s = value.c_str();
        Serial.println(s);
        /*
        for (int i = 0; i < 2; i++){
          char a = s.charAt(i);
          int num = (int)a;
          String msg = (String(i)+":"+s+":"+String(num));
          Serial.println(msg);
        }
        */
          

        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(115200);

  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("M5Stack");
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

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
