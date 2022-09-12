
void initMlx(){
  if (!mlx.begin()) {
    Serial.println("Could not find a valid MLX sensor, check wiring!");
    while (1);
  }
}
//void onBeatDetected()
//{
//    Serial.println("Beat!");
//}
//MAX30100 sensor;
//
//void configureMax30100() {
//  sensor.setMode(MAX30100_MODE_SPO2_HR);
//  sensor.setLedsCurrent(MAX30100_LED_CURR_50MA, MAX30100_LED_CURR_50MA);
//  sensor.setLedsPulseWidth(MAX30100_SPC_PW_1600US_16BITS);
//  sensor.setSamplingRate(MAX30100_SAMPRATE_100HZ);
//  sensor.setHighresModeEnabled(true);
//}
//void initMax()
//
//{
//  sensor.begin();
//  if (!pox.begin()) {
//        Serial.println("FAILED");
//        for(;;);
//  }
//      pox.setOnBeatDetectedCallback(onBeatDetected);
//      configureMax30100();
//
//}

long meanDiff(long M) {
  #define LM_SIZE 5
  static  long LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static  long sum = 0;
  static byte count = 0;
  long  avg = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  avg = sum / count;
  return avg - M;
}
