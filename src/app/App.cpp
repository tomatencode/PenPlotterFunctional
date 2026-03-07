#include "App.hpp"

#include "storage/FileSystem.hpp"
#include "jobManager/JobManager.hpp"
#include "webInterface/WebInterface.hpp"
#include "ui/UI.hpp"

#include <LCD-I2C.h>
#include "hardware/display/LcdDisplay.hpp"
#include "hardware/rotaryEncoder/RotaryEncoder.hpp"
#include "config/pins.hpp"
#include "config/ui_config.hpp"

JobManager jobManager;
LCD_I2C lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);
LcdDisplay display(lcd);

RotaryEncoder encoder(ENCODER_DT_PIN, ENCODER_CLK_PIN, ENCODER_SW_PIN, ENCODER_DEBOUNCE_MS);


UI ui(display, encoder);

void appInit()
{
    Serial.println("Initializing lcd...");
    Wire.begin();
    lcd.begin(&Wire);
    lcd.display();
    lcd.backlight();

    encoder.begin();

    fsInit();
    webInit();
}

void appUpdate()
{
    webUpdate();
    ui.update();
    jobManager.jobManagerUpdate();
}