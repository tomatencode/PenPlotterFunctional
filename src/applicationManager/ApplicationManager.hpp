#pragma once

#include "jobManager/JobManager.hpp"
#include "webInterface/WebInterface.hpp"
#include "storage/FileManager.hpp"
#include "ui/UiManager.hpp"
#include "systemServices/MotionState.hpp"

class ApplicationManager
{
public:
    ApplicationManager(MotionState& ms);
    
    void init();
    void update();

private:

    LCD_I2C lcd;
    LcdDisplay display;
    RotaryEncoder encoder;
    Buzzer buzzer;

    JobManager jobManager;
    FileManager fileManager;
    WebInterface webInterface;
    ui::UiManager uiManager;

    MotionState& ms;
};