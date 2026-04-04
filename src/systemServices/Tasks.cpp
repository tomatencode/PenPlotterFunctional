#include "Tasks.hpp"

#include "FreeRtosQueue.hpp"
#include "GcodeMessage.hpp"
#include "MotionState.hpp"
#include "RuntimeSettings.hpp"
#include "applicationManager/ApplicationManager.hpp"
#include "plottingManager/PlottingManager.hpp"
#include "settings/SettingsRepository.hpp"

TaskHandle_t motionTaskHandle = nullptr;
TaskHandle_t systemTaskHandle = nullptr;

/*
    CORE 1
    Motion execution task
*/
void motionTask(void *parameter)
{
    Serial.print("Motion task running on core: ");
    Serial.println(xPortGetCoreID());

    PlottingManager* plottingManager =
        static_cast<PlottingManager*>(parameter);

    plottingManager->init();

    while (true)
    {
        plottingManager->update();
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

    ApplicationManager* app =
        static_cast<ApplicationManager*>(parameter);

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
void startSystemTasks()
{
    size_t gcodeQueueSize = 32;
    static FreeRtosQueue<GcodeMessage> gcodeQueue(gcodeQueueSize);
    static MotionState motionState;
    static RuntimeSettings runtimeSettings;
    static SettingsRepository settingsRepository(runtimeSettings);
    
    // Load settings from NVS at startup
    settingsRepository.init();
    
    static ApplicationManager appManager(motionState, gcodeQueue, settingsRepository);
    static PlottingManager plottingManager(motionState, gcodeQueue, runtimeSettings);

    // Motion task (CORE 1)
    xTaskCreatePinnedToCore(
        motionTask,
        "MotionTask",
        10000,
        &plottingManager,
        2,
        &motionTaskHandle,
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