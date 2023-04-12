#define ARR_SIZE 256
uint8_t* arr = new uint8_t[ARR_SIZE];

TaskHandle_t SerialWriterTask_Handle;
void SerialWriterTask (void* pvParam) {
  for(;;) {
    vTaskSuspend(NULL);
    Serial.write(arr, ARR_SIZE);
    Serial.write('\n');
  }
}

void ReaderTask (void* pvParam) {
  int i = 0;
  for(;;) {
    arr[i] = analogRead(A0);
    i++;
    if (i == ARR_SIZE) {
      vTaskResume(SerialWriterTask_Handle);
      i = 0;
    }
    vTaskDelay(5 / portTICK_RATE_MS);
  }
}

void setup() {
  Serial.begin(115200);
  arr[255] = 228;
  analogReadResolution(8);
  xTaskCreate(SerialWriterTask, 
              "Write arr to Serial", 
              2048,
              NULL,
              2, 
              &SerialWriterTask_Handle);


  xTaskCreate(ReaderTask, 
              "Read ADC to arr", 
              2048,
              NULL,
              2, 
              NULL);
}

void loop() {
  // uint8_t* arr = new uint8_t[ARR_SIZE];
  // for (size_t i = 0; i < ARR_SIZE; i++) {
  //   arr[i] = (i % 80) + 32;
  // }
  // //arr[ARR_SIZE - 1] = 10;
  // Serial.write(arr, ARR_SIZE);
  // Serial.write('\n');
  // delete[] arr;

  //delay(1000);
}
