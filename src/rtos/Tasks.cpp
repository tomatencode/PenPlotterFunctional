#include "Tasks.hpp"

#include "RtosQueue.hpp"
#include "GcodeMessage.hpp"
#include "MotionState.hpp"
#include "systemController/SystemController.hpp"
#include "plottingController/PlottingController.hpp"
#include "settings/RuntimeSettings.hpp"
#include "settings/SettingPersistence.hpp"

TaskHandle_t plottingTaskHandle = nullptr;
TaskHandle_t systemTaskHandle = nullptr;

/*
    CORE 1
    Plotting task
*/
void plottingTask(void *parameter)
{
    Serial.print("Plotting task running on core: ");
    Serial.println(xPortGetCoreID());

    PlottingController* plottingController =
        static_cast<PlottingController*>(parameter);

    plottingController->init();

    while (true)
    {
        plottingController->update();
        taskYIELD();
    }
}

/*
    CORE 0
    System task
*/
void systemTask(void *parameter)
{
    Serial.print("System task running on core: ");
    Serial.println(xPortGetCoreID());

    SystemController* app =
        static_cast<SystemController*>(parameter);

    app->init();

    while (true)
    {
        app->update();

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

/*
    Create all tasks
*/
void startRtosTasks()
{
    size_t gcodeQueueSize = 32;
    static RtosQueue<GcodeMessage> gcodeQueue(gcodeQueueSize);
    static MotionState motionState;
    static RuntimeSettings runtimeSettings;
    static SettingPersistence settingPercistence(runtimeSettings);
    
    // Load settings from NVS at startup
    settingPercistence.init();

    static SystemController appManager(motionState, gcodeQueue, settingPercistence, runtimeSettings);
    static PlottingController plottingController(motionState, gcodeQueue, settingPercistence, runtimeSettings);

    // Plotting task (CORE 1)
    xTaskCreatePinnedToCore(
        plottingTask,
        "PlottingTask",
        10000,
        &plottingController,
        2,
        &plottingTaskHandle,
        1);

    // System task (CORE 0)
    xTaskCreatePinnedToCore(
        systemTask,
        "SystemTask",
        6000,
        &appManager,
        1,
        &systemTaskHandle,
        0);
}