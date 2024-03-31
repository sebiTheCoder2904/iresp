#include <Arduino.h>
#include <string.h>
#include <NimBLEDevice.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID               "b1ec5ab0-f818-4398-b3d3-b9fe79391b34"
#define CHARACTERISTIC_UUID_read   "b1ec5ab1-f818-4398-b3d3-b9fe79391b34"
#define CHARACTERISTIC_UUID_write  "b1ec5ab2-f818-4398-b3d3-b9fe79391b34"

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
  pCharacteristic_read = pService->createCharacteristic(CHARACTERISTIC_UUID_read, NIMBLE_PROPERTY::READ);
  pCharacteristic_write = pService->createCharacteristic(CHARACTERISTIC_UUID_write, NIMBLE_PROPERTY::WRITE);

  pCharacteristic_read->setValue(4);
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
  //Serial.println("sent '" + String(x) + "'");

  String now_read_value = String(pCharacteristic_write->getValue());
  if (!(old_read_value == now_read_value))  {
    Serial.println(String(now_read_value));
    old_read_value = now_read_value;
  }

  x++;
  delay(25);
}