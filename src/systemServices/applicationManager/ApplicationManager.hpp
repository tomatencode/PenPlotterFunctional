#pragma once

#include <LCD-I2C.h>
#include "jobController/JobController.hpp"
#include "webInterface/WebInterface.hpp"
#include "storage/FileManager.hpp"
#include "ui/UiOrchestrator.hpp"
#include "ui/InputMapper.hpp"
#include "ui/framework/router/Router.hpp"
#include "ui/framework/render/Render.hpp"
#include "systemServices/MotionState.hpp"
#include "systemServices/FreeRtosQueue.hpp"
#include "systemServices/GcodeMessage.hpp"
#include "settings/SettingPercistence.hpp"
#include "settings/RuntimeSettings.hpp"

class ApplicationManager
{
public:
    ApplicationManager(MotionState& motionState, FreeRtosQueue<GcodeMessage>& gcodeQueue,
                       SettingPercistence& settingPercistence, RuntimeSettings& runtimeSettings);
    
    void init();
    void update();

private:

    LCD_I2C _lcd;
    LcdDisplay _display;
    RotaryEncoder _encoder;
    Buzzer _buzzer;

    JobController _jobController;
    FileManager _fileManager;
    WebInterface _webInterface;

    ui::Router _router;
    ui::Renderer _renderer;
    ui::InputMapper _inputMapper;
    ui::UiOrchestrator _uiOrchestrator;
};