#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#define TIME_UPDATE 1000

TaskHandle_t S1, S2, S3;
SemaphoreHandle_t mutex;

//void readInput(void *pvParameters);
//void count100(void *pvParameters);

void setup()
{
  Serial.begin(9600);
  mutex = xSemaphoreCreateMutex();
  xTaskCreate(readInput, "input", 128, NULL, 2, NULL);
  xTaskCreate(count100, "100", 128, NULL, 1, &S1);
  xTaskCreate(count200, "200", 128, NULL, 1, &S2);
  xTaskCreate(count300, "300", 128, NULL, 1, &S3);
  vTaskStartScheduler();
}

void loop(){}

void displayValue(int count, int button)
{
  for(int i=0; i < count; i++)
  {
    delay(TIME_UPDATE);
    Serial.println((i+1) + button*1000);
  }
}

void count100(void *pvParameters)
{
  while(1)
  {
    int count = 10;
    char btn;
    if((xSemaphoreTake(mutex, 1000/portTICK_PERIOD_MS)) == pdTRUE)
    {
      displayValue(count, 1);
      xSemaphoreGive(mutex);
      vTaskSuspend(S1);
    }
  }
}

void count200(void *pvParameters)
{
  while(1)
  {
    int count = 15;
    if((xSemaphoreTake(mutex, 1000/portTICK_PERIOD_MS)) == pdTRUE)
    {
      displayValue(count, 2);
      xSemaphoreGive(mutex);
      vTaskSuspend(S2);
    }
  }
}

void count300(void *pvParameters)
{
  while(1)
  {
    int count = 20;
    if((xSemaphoreTake(mutex, 1000/portTICK_PERIOD_MS)) == pdTRUE)
    {
      displayValue(count, 3);
      xSemaphoreGive(mutex);
      vTaskSuspend(S3);
    }
  }
}

void readInput(void *pvParamenters)
{
  char press, release;
  int accPress = 0;
  Serial.println("E = S1 || R = S2 || T = S3");
  vTaskSuspend(S1);
  vTaskSuspend(S2);
  vTaskSuspend(S3);
  while(1)
  {
    press = Serial.read();

    if(press == 'E')
    {
      vTaskResume(S1);
      accPress++;
      if(accPress >= 5)
      {
        Serial.println("Button overload!");
        vTaskSuspend(S1);
        accPress = 0;
      }
    }

    if(press == 'R')
    {
      vTaskResume(S2);
      accPress++;
      if(accPress >= 5)
      {
        Serial.println("Button overload!");
        vTaskSuspend(S2);
        accPress = 0;
      }
    }

    if(press == 'T')
    {
      vTaskResume(S3);
      accPress++;
      if(accPress >= 5)
      {
        Serial.println("Button overload!");
        vTaskSuspend(S3);
        accPress = 0;
      }
    }

    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}
