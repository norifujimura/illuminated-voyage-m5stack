// Bluetooth LE
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


///////////////////
// Bluetooth LE  //
///////////////////
BLEServer *pServer = NULL;
BLECharacteristic * pNotifyCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
bool messageReceived = false;
String message;


// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID                "7a4a668e-0f71-11ec-82a8-0242ac130003"
#define CHARACTERISTIC_UUID_RX      "7a4a668e-0f71-11ec-82a8-0242ac130003"
#define CHARACTERISTIC_UUID_NOTIFY  "7a4a668e-0f71-11ec-82a8-0242ac130003"

// Bluetooth LE Change Connect State
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

// Bluetooth LE Recive
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        String cmd = String(rxValue.c_str());
        Serial.print("Received Value: ");
        Serial.println(cmd);
        message = cmd;
        messageReceived = true;
        /*
        if (cmd == "RED")
        {
          // RED
          lastColor = "RED";
          updateColor = true;
        }
        if (cmd == "YELLOW")
        {
          // YELLOW
          lastColor = "YELLOW";
          updateColor = true;
        }
        if (cmd == "BLUE")
        {
          // BLUE
          lastColor = "BLUE";
          updateColor = true;
        }
        */
      }
    }
};


// Bluetooth LE initialize
void initBLE() {
  // Create the BLE Device
  BLEDevice::init(LOCAL_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pNotifyCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_NOTIFY,
                        BLECharacteristic::PROPERTY_NOTIFY
                        );
  
  pNotifyCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}

// Bluetooth LE loop
void loopBLE() {
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("startAdvertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

bool isBleDeviceConnected(){
  return deviceConnected;
}

void sendBleMessage(String message){
  char sendMessage[10];
  message.toCharArray(sendMessage, 10);
  pNotifyCharacteristic->setValue(sendMessage);
  pNotifyCharacteristic->notify();
}


String getBleMessage(){
  if(messageReceived){
    messageReceived = false;
    return message;
  }else{
    return "";
  }
}
