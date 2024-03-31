#include <Arduino.h>
#include <string.h>
#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID               "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_read   "6e400002-b5a3-f393-e0a9-e50e24dcca9e"
#define CHARACTERISTIC_UUID_write  "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

int x;
String old_read_value;

BLECharacteristic *pCharacteristic_read;
BLECharacteristic *pCharacteristic_write;

void setup() {
  x = 0;
  old_read_value = ".";
  Serial.begin(9600);
  Serial.println("Starting BLE work in 5 sec");
  delay(5000);
  Serial.println("Starting BLE work now");

  BLEDevice::init("Esp32c3");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic_read = pService->createCharacteristic(CHARACTERISTIC_UUID_read, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);
  pCharacteristic_write = pService->createCharacteristic(CHARACTERISTIC_UUID_write, NIMBLE_PROPERTY::WRITE);

  pCharacteristic_read->setValue(4);

  // Add CCCD descriptor to enable notifications
  pCharacteristic_read->addDescriptor(new NimBLE2904());

  pCharacteristic_write->setValue("test");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);


  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  pCharacteristic_read->setValue(String(x) + "\n");
  pCharacteristic_read->notify(); // Notify clients of the value change
  //Serial.println("sent '" + String(x) + "'");

  String now_read_value = String(pCharacteristic_write->getValue());
  if (!(old_read_value == now_read_value))  {
    Serial.println(String(now_read_value));
    old_read_value = now_read_value;
  }

  x++;
  delay(25);
}