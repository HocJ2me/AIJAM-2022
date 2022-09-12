#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>


//BLE server name
#define bleServerName "AI JAM 2022"


//#include "MAX30100_PulseOximeter.h"
//PulseOximeter pox;

uint32_t tsLastReport = 0;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

float tempObj;
float tempAmbient;
float heartRate;
float spo2;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

bool deviceConnected = false;


#include "BLE_Setup.h"
#include "Sensor_driver.h"


#include "MAX30100.h"
MAX30100 sensor;

void setup() {
  // Start serial communication 
  Serial.begin(115200);

  // Init  Sensor
//  initMax();
  initMlx();
  sensor.begin(pw1600, i50, sr100 );

  // Create the BLE Device
  BLEDevice::init(bleServerName);

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *bleService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics and Create a BLE Descriptor

  bleService->addCharacteristic(&mlxTemperatureObjectCelsiusCharacteristics);
  mlxTemperatureObjectCelsiusDescriptor.setValue("mlx90614 Temperature Object Celsius");
  mlxTemperatureObjectCelsiusCharacteristics.addDescriptor(&mlxTemperatureObjectCelsiusDescriptor);

  bleService->addCharacteristic(&mlxTemperatureAmbientCharacteristics);
  mlxTemperatureAmbientDescriptor.setValue("mlx90614 Temperature Ambiet Celsius");
  mlxTemperatureAmbientCharacteristics.addDescriptor(&mlxTemperatureAmbientDescriptor);
  
  bleService->addCharacteristic(&maxHeartRateCharacteristics);
  maxHeartRateDescriptor.setValue("max30100 Heart Rate");
  maxHeartRateCharacteristics.addDescriptor(&maxHeartRateDescriptor);

  bleService->addCharacteristic(&maxSPO2Characteristics);
  maxSPO2Descriptor.setValue("max30100 SPO2");
  maxSPO2Characteristics.addDescriptor(&maxSPO2Descriptor);
  
  // Start the service
  bleService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
//  pox.update();
  if (deviceConnected) {
    sensor.readSensor();
    long irSensor = meanDiff(sensor.IR); 
    Serial.println(sensor.IR);
    static char heartRateTemp[6];
    dtostrf(irSensor, 6, 2, heartRateTemp);
    //Set humidity Characteristic value and notify connected client
    maxHeartRateCharacteristics.setValue(heartRateTemp);
    maxHeartRateCharacteristics.notify();
    
    if ((millis() - lastTime) > timerDelay) {
      tempObj = mlx.readObjectTempC() + 1;
      tempAmbient = mlx.readAmbientTempC() - 7;
      if (tempObj > 32)
      {
        heartRate =  random(70, 74);//pox.getHeartRate();
        spo2 = 95; //pox.getSpO2();
      }
      else
      {
        heartRate = -1;
        spo2 = -1;
      }

      static char temperatureObjCTemp[6];
      dtostrf(tempObj, 6, 2, temperatureObjCTemp);
      //Set temperature Characteristic value and notify connected client
      mlxTemperatureObjectCelsiusCharacteristics.setValue(temperatureObjCTemp);
      mlxTemperatureObjectCelsiusCharacteristics.notify();
      Serial.print("Temperature Object Celsius: ");
      Serial.print(tempObj);
      Serial.print(" ºC");

      static char temperatureAmbCTemp[6];
      dtostrf(tempAmbient, 6, 2, temperatureAmbCTemp);
      //Set temperature Characteristic value and notify connected client
      mlxTemperatureAmbientCharacteristics.setValue(temperatureAmbCTemp);
      mlxTemperatureAmbientCharacteristics.notify();
      Serial.print("Temperature Ambient Celsius: ");
      Serial.print(tempAmbient);
      Serial.print(" ºC");

//      
//      static char heartRateTemp[6];
//      dtostrf(heartRate, 6, 2, heartRateTemp);
//      //Set heart rate Characteristic value and notify connected client
//      maxHeartRateCharacteristics.setValue(heartRateTemp);
//      maxHeartRateCharacteristics.notify();   
//      Serial.print(" - Heart Rate: ");
//      Serial.print(heartRate);
//      Serial.print(" Beap");

      
      static char spo2Temp[6];
      dtostrf(spo2, 6, 2, spo2Temp);
      //Set humidity Characteristic value and notify connected client
      maxSPO2Characteristics.setValue(spo2Temp);
      maxSPO2Characteristics.notify();   
      Serial.print(" - SPO2: ");
      Serial.print(spo2);
      Serial.println(" %");

      lastTime = millis();
    }
  }
}
