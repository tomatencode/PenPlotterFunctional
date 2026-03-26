#include "ApplicationManager.hpp"

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
      _jobController(motionState, gcodeQueue, _fileManager),
      _webInterface(_jobController, motionState, _fileManager),
      _router(),
      _renderer(_display),
      _inputMapper(_encoder),
      _UiOrchestrator(_router, _renderer, _inputMapper, _jobController, _fileManager, motionState, _buzzer,
                        [this]() -> bool {
                            return _webInterface.isWiFiConnected();
                        }
                    )
{}

void ApplicationManager::init()
{
    Wire.begin();

    _lcd.begin(&Wire);
    _lcd.display();
    _lcd.backlight();

    _buzzer.begin();
    _encoder.begin();

    _fileManager.init();

    _UiOrchestrator.init();
    _webInterface.init();

    _buzzer.playMelody(startupMelody);
}

void ApplicationManager::update()
{
    _webInterface.update();
    _jobController.update();
    _buzzer.update();
    _UiOrchestrator.update();
}