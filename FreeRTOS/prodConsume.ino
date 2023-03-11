#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <TimerOne.h>
#include <MultiFuncShield.h>

SemaphoreHandle_t binSem;

int sharedBuffer = 0;
void setup() {
  //Shield initialization
  Timer1.initialize();
  MFS.initialize(&Timer1);
  Serial.begin(9600);
  //The Binary Semaphore
  binSem = xSemaphoreCreateMutex();
  //Create the tasks
  xTaskCreate (taskProd, "Producer", 128, NULL, 1, NULL);
  xTaskCreate (taskCons, "Consumer", 128, NULL, 1, NULL);
  xSemaphoreGive(binSem);
  vTaskStartScheduler();
}

void loop()
{
  
}

//Producer task
void taskProd (void *pvParameters)
{
 while(1)
 {
 //Take mutex, but do not wait more than 10 ticks
 if (xSemaphoreTake(binSem, 10) == pdTRUE)
 {
 int produce = sharedBuffer;
 if (produce == 0)
 produce++;

 //Simulate some FAST processing to produce something
 vTaskDelay (random(10,100)/ portTICK_PERIOD_MS);
 sharedBuffer = produce;

 Serial.print(pcTaskGetName(xTaskGetCurrentTaskHandle()));
 Serial.print(": Buffer is ");
 Serial.println(sharedBuffer);
 //Give mutex
 xSemaphoreGive(binSem);
 }
 taskYIELD();
 }
}

void taskCons (void *pvParameters)
{
  while(1)
  {
    if(xSemaphoreTake(binSem, 10) == pdTRUE)
    {
      int consume = sharedBuffer;
      if(consume == 1)
      {
        consume--;
      }

      vTaskDelay (random(10,100)/ portTICK_PERIOD_MS);
      sharedBuffer = consume;

      Serial.print(pcTaskGetName(xTaskGetCurrentTaskHandle()));
      Serial.print(": Buffer is ");
      Serial.println(sharedBuffer);
      //Give mutex
      xSemaphoreGive(binSem);
    }
    taskYIELD();
  }
}
