#include <Arduino.h>
#include "NeuralNetwork.h"

#define LED 2
#define WINDOW_SIZE 200
#define OVERLAPPING_SIZE 160

NeuralNetwork *nn;
float signal_buffer[WINDOW_SIZE] = {0};

TaskHandle_t PredicterTask_Handle;
void PredicterTask (void* pvParam) {
  for(;;) {
    vTaskSuspend(NULL);

    for(int i = 0; i < WINDOW_SIZE; i++) {
      nn->getInputBuffer()[i] = signal_buffer[i]; //copy buffer to nn input
    }

    float result = nn->predict();
    if (result > 0.5) {
      digitalWrite(LED, HIGH);
    }

    else {
      digitalWrite(LED, LOW);
    }

    for (int i = 0; i < OVERLAPPING_SIZE; i++) {
		  signal_buffer[i] = signal_buffer[i + (WINDOW_SIZE - OVERLAPPING_SIZE)];  //rewrite overlap
	  }
  }
}

void ReaderTask(void* pvParam) {
  int i = OVERLAPPING_SIZE;
  for(;;) {
    signal_buffer[i] = analogRead(A0);
    i++;
    if (i == WINDOW_SIZE) {
      vTaskResume(PredicterTask_Handle);
      i = OVERLAPPING_SIZE;
    }
    vTaskDelay(5 / portTICK_RATE_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  nn = new NeuralNetwork();

  analogReadResolution(8);
  xTaskCreate(PredicterTask, 
              "Predicting and switching LED task", 
              2048,
              NULL,
              2, 
              &PredicterTask_Handle);


  xTaskCreate(ReaderTask, 
              "Read ADC to arr", 
              2048,
              NULL,
              2, 
              NULL);
}


void loop()
{
}