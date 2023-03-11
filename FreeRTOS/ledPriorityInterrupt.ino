#include <Arduino_FreeRTOS.h>
#include <TimerOne.h>
#include <MultiFuncShield.h>
void taskBlink1 (void *pvParameters);
void taskPrint (void *pvParameters);

void setup() {
 //Initialize the shield
 Timer1.initialize();
 MFS.initialize(&Timer1);
//Initialize the Serial interface
 Serial.begin(9600);
 xTaskCreate (taskBlink1, //Function
 "Led1", //Description
 128, //Stack "size"
 NULL, //Parameters for the function
 1, //Priority
 NULL); //Task handler
 //xTaskCreate (taskPrint, "Print", 128, NULL, 1, NULL);
 xTaskCreate (taskInterrupt, "Interrupt", 128, NULL, 2, NULL);
 vTaskStartScheduler();
}

void loop()
{
  
}

void taskBlink1 (void *pvParameters)
{
 while(1)
 {
 MFS.writeLeds (LED_1, ON);
 vTaskDelay (500 / portTICK_PERIOD_MS);
 MFS.writeLeds (LED_1, OFF);
 vTaskDelay (500 / portTICK_PERIOD_MS);
 }
}

void taskInterrupt(void *pvParameters)
{
  while(1)
  {
    if(digitalRead(A1)==LOW)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  }
}
