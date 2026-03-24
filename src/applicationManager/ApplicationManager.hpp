#pragma once

#include "jobManager/JobManager.hpp"
#include "webInterface/WebInterface.hpp"
#include "storage/FileManager.hpp"
#include "ui/UiManager.hpp"
#include "systemServices/MotionStateManager.hpp"

class ApplicationManager
{
public:
    ApplicationManager(MotionStateManager& ms);
    
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

    MotionStateManager& ms;
};