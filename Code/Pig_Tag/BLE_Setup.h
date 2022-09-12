
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "6d636a08-09b5-11ed-861d-0242ac120002"

// Temperature Characteristic and Descriptor

BLECharacteristic mlxTemperatureObjectCelsiusCharacteristics("1db391a2-09bc-11ed-861d-0242ac120002", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor mlxTemperatureObjectCelsiusDescriptor(BLEUUID((uint16_t)0x2902));

BLECharacteristic mlxTemperatureAmbientCharacteristics("2d9ef5d4-09bc-11ed-861d-0242ac120002", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor mlxTemperatureAmbientDescriptor(BLEUUID((uint16_t)0x2903));

BLECharacteristic maxHeartRateCharacteristics("33d84cac-09bc-11ed-861d-0242ac120002", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor maxHeartRateDescriptor(BLEUUID((uint16_t)0x2904));

BLECharacteristic maxSPO2Characteristics("3afab182-09bc-11ed-861d-0242ac120002", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor maxSPO2Descriptor(BLEUUID((uint16_t)0x2905));

//Setup callbacks onConnect and onDisconnect
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};
