#include "Tasks.hpp"

#include "../systemServices/Queues.hpp"
#include "../motion/GCodeParser.hpp"
#include "../interface/App.hpp"
#include "../motion/Machine.hpp"

extern GCodeParser gcodeParser;
extern std::vector<String> gcodeLines;

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
    GcodeMessage msg;

    Serial.print("Motion task running on core:");
    Serial.println(xPortGetCoreID());

    // Initialize all motion-related subsystems
    initMachine();

    while (true)
    {
        if (xQueueReceive(gcodeQueue, &msg, portMAX_DELAY) == pdTRUE)
        {
            Serial.print("Executing line ");
            Serial.print(msg.lineNumber);
            Serial.print(": ");
            Serial.println(msg.line);

            // if you implement shared motionState later
            // motionState.currentLineNumber = msg.lineNumber;

            gcodeParser.executeLine(msg.line);
        }
    }
}

/*
    CORE 0
    System task

    Handles:
    - UI (encoder + LCD)
    - Web interface (upload/list/start/stop jobs)
    - Job manager (streams files to motion queue)
*/
void systemTask(void *parameter)
{
    Serial.print("System task running on core: ");
    Serial.println(xPortGetCoreID());

    // Initialize all Core 0 subsystems
    appInit();

    while (true)
    {
        // Non-blocking updates
        appUpdate();  // updates: web interface, UI, JobManager

        // Small delay to yield CPU, adjust if needed
        vTaskDelay(10 / portTICK_PERIOD_MS);
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