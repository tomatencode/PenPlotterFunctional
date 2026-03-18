#include "App.hpp"

#include "storage/FileSystem.hpp"
#include "jobManager/JobManager.hpp"
#include "webInterface/WebInterface.hpp"
#include "ui/UI.hpp"

#include <LCD-I2C.h>
#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "hardware/buzzer/Buzzer.hpp"
#include "config/pins.hpp"
#include "config/ui_config.hpp"

JobManager jobManager;
WebInterface webInterface;

LCD_I2C lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);
LcdDisplay display(lcd);

RotaryEncoder encoder(ENCODER_DT_PIN, ENCODER_CLK_PIN, ENCODER_SW_PIN, ENCODER_DEBOUNCE_MS);

Buzzer buzzer(BUZZER_PIN, 5);

UI uiManager(display, encoder, buzzer);

const Buzzer::Melody startupMelody((uint16_t[]){262, 294, 330}, (uint16_t[]){200, 200, 200});

void appInit()
{
    // Initialize lcd
    Wire.begin();
    lcd.begin(&Wire);
    lcd.display();
    lcd.backlight();

    // Initialize buzzer
    buzzer.begin();

    // Initialize encoder
    encoder.begin();

    fsInit();
    uiManager.init();
    webInterface.init();

    Serial.println("App initialized.");
    buzzer.playMelody(startupMelody);
}

void appUpdate()
{
    webInterface.update();
    jobManager.jobManagerUpdate();
    buzzer.update();
    uiManager.update();
}