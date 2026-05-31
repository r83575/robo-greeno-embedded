#include <Arduino.h>

#define BUTTON_PIN  18
#define LED_PIN     2

QueueHandle_t buttonQueue;

void buttonTask(void *pvParameters)
{
    int lastState = HIGH;

    while (true)
    {
        int currentState = digitalRead(BUTTON_PIN);

        if (currentState != lastState)
        {
            int value;

            if (currentState == LOW)
            {
                value = 1;
                Serial.println("Sent: 1");
            }
            else
            {
                value = 0;
                Serial.println("Sent: 0");
            }

            xQueueSend(buttonQueue, &value, portMAX_DELAY);
            lastState = currentState;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void ledTask(void *pvParameters)
{
    int receivedValue;

    while (true)
    {
        if (xQueueReceive(buttonQueue, &receivedValue, portMAX_DELAY) == pdTRUE)
        {
            Serial.print("Received: ");
            Serial.println(receivedValue);

            if (receivedValue == 1)
            {
                digitalWrite(LED_PIN, HIGH);
            }
            else
            {
                digitalWrite(LED_PIN, LOW);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void setup()
{
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    buttonQueue = xQueueCreate(10, sizeof(int));

    if (buttonQueue == NULL)
    {
        Serial.println("Queue creation failed");
        while (true);
    }

    xTaskCreate(
        buttonTask,
        "Button Task",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        ledTask,
        "LED Task",
        2048,
        NULL,
        1,
        NULL
    );
}

void loop()
{
}