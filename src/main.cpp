#include <Arduino.h>
#include <WiFi.h>
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

String printMacAddress() {
  WiFi.mode(WIFI_OFF); // turn off Wifi to access MAC address
  delay(10);
  
  // Get MAC address
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_WIFI_STA);
  
  // Convert MAC address to string
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  
  return String(macStr);
}


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

    if (now_read_value == "macaddress") {
      String macAddress = printMacAddress();
      pCharacteristic_read->setValue(macAddress);
    }
  }

  x++;
  delay(25);
}