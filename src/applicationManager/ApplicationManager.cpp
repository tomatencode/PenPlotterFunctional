#include "ApplicationManager.hpp"

#include "storage/FileSystem.hpp"
#include "jobManager/JobManager.hpp"
#include "webInterface/WebInterface.hpp"
#include "ui/UiManager.hpp"

#include <LCD-I2C.h>
#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "hardware/buzzer/Buzzer.hpp"
#include "config/pins.hpp"
#include "config/ui_config.hpp"

const Buzzer::Melody startupMelody((uint16_t[]){262, 294, 330}, (uint16_t[]){200, 200, 200});

ApplicationManager::ApplicationManager(MotionStateManager& ms)
    : ms(ms),
      lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS),
      display(lcd),
      encoder(ENCODER_DT_PIN, ENCODER_CLK_PIN, ENCODER_SW_PIN, ENCODER_DEBOUNCE_MS),
      buzzer(BUZZER_PIN, 5),
      jobManager(ms),
      webInterface(jobManager),
      uiManager(jobManager, display, encoder, buzzer)
{
}

void ApplicationManager::init()
{
    Wire.begin();

    lcd.begin(&Wire);
    lcd.display();
    lcd.backlight();

    buzzer.begin();
    encoder.begin();

    storage::fsInit();

    uiManager.init();
    webInterface.init();
}

void ApplicationManager::update()
{
    webInterface.update();
    jobManager.update();
    buzzer.update();
    uiManager.update();
}