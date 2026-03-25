#pragma once

#include "jobManager/JobManager.hpp"
#include "webInterface/WebInterface.hpp"
#include "storage/FileManager.hpp"
#include "ui/UiManager.hpp"
#include "systemServices/MotionState.hpp"
#include "systemServices/FreeRtosQueue.hpp"
#include "systemServices/GcodeMessage.hpp"

class ApplicationManager
{
public:
    ApplicationManager(MotionState& ms, FreeRtosQueue<GcodeMessage>& gcodeQueue);
    
    void init();
    void update();

private:

    LCD_I2C _lcd;
    LcdDisplay _display;
    RotaryEncoder _encoder;
    Buzzer _buzzer;

    JobManager _jobManager;
    FileManager _fileManager;
    WebInterface _webInterface;
    ui::UiManager _uiManager;

    MotionState& _motionState;
    FreeRtosQueue<GcodeMessage>& _gcodeQueue;
};