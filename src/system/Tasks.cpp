#include "Tasks.hpp"
#include "Queues.hpp"

#include "../gcode/GCodeParser.hpp"

extern GCodeParser gcodeParser;
extern std::vector<String> gcodeLines;

#define MAX_GCODE_LINE 96

TaskHandle_t motionTaskHandle = nullptr;
TaskHandle_t systemTaskHandle = nullptr;

/*
    CORE 1
    Motion execution task

    This must stay deterministic and should never run
    WiFi, filesystem or UI code.
*/
void motionTask(void *parameter)
{
    char line[MAX_GCODE_LINE];

    Serial.println("Motion task running on core:");
    Serial.println(xPortGetCoreID());

    while (true)
    {
        if (xQueueReceive(gcodeQueue, &line, portMAX_DELAY) == pdTRUE)
        {
            Serial.print("Executing: ");
            Serial.println(line);

            gcodeParser.executeLine(line);
        }
    }
}

/*
    CORE 0
    System task

    Handles:
    - UI
    - filesystem
    - web interface
    - serial commands

    For now it only pushes test G-code.
*/
void systemTask(void *parameter)
{
    Serial.println("System task running on core:");
    Serial.println(xPortGetCoreID());

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    for (const auto &l : gcodeLines)
    {
        char line[MAX_GCODE_LINE];

        strncpy(line, l.c_str(), MAX_GCODE_LINE);
        line[MAX_GCODE_LINE - 1] = '\0';

        Serial.print("Queueing: ");
        Serial.println(line);

        xQueueSend(gcodeQueue, &line, portMAX_DELAY);
    }

    while (true)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/*
    Create all tasks and pin them to cores
*/
void startSystemTasks()
{
    // Motion task (CORE 1)
    xTaskCreatePinnedToCore(
        motionTask,
        "MotionTask",
        10000,
        NULL,
        2,
        &motionTaskHandle,
        1);

    // System task (CORE 0)
    xTaskCreatePinnedToCore(
        systemTask,
        "SystemTask",
        6000,
        NULL,
        1,
        &systemTaskHandle,
        0);
}