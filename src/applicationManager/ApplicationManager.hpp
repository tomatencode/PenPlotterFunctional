#pragma once

#include "jobManager/JobManager.hpp"
#include "webInterface/WebInterface.hpp"
#include "ui/UiManager.hpp"

class ApplicationManager
{
public:
    ApplicationManager();
    
    void init();
    void update();

private:

    LCD_I2C lcd;
    LcdDisplay display;
    RotaryEncoder encoder;
    Buzzer buzzer;

    JobManager jobManager;
    WebInterface webInterface;
    ui::UiManager uiManager;
};