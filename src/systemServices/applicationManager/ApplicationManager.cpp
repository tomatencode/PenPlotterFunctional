#include "ApplicationManager.hpp"

#include "storage/FileManager.hpp"
#include "jobController/JobController.hpp"
#include "webInterface/WebInterface.hpp"
#include "ui/UiManager.hpp"

#include <LCD-I2C.h>
#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "hardware/buzzer/Buzzer.hpp"
#include "config/pins.hpp"
#include "config/ui_config.hpp"

const Buzzer::Melody startupMelody((uint16_t[]){262, 294, 330}, (uint16_t[]){200, 200, 200});

ApplicationManager::ApplicationManager(MotionState& motionState, FreeRtosQueue<GcodeMessage>& gcodeQueue)
    : _motionState(motionState),
      _gcodeQueue(gcodeQueue),
      _lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS),
      _display(_lcd),
      _encoder(ENCODER_DT_PIN, ENCODER_CLK_PIN, ENCODER_SW_PIN, ENCODER_DEBOUNCE_MS),
      _buzzer(BUZZER_PIN, 5),
      _fileManager(),
      _jobController(_motionState, _gcodeQueue, _fileManager),
      _webInterface(_jobController, _motionState, _fileManager),
      _uiManager(_jobController, _motionState, _fileManager, _display, _encoder, _buzzer)
{
}

void ApplicationManager::init()
{
    Wire.begin();

    _lcd.begin(&Wire);
    _lcd.display();
    _lcd.backlight();

    _buzzer.begin();
    _encoder.begin();

    _fileManager.init();

    _uiManager.init();
    _webInterface.init();

    _buzzer.playMelody(startupMelody);
}

void ApplicationManager::update()
{
    _webInterface.update();
    _jobController.update();
    _buzzer.update();
    _uiManager.update();
}