#include "UI.hpp"
#include "render/CustomChars.hpp"
#include <string>

int bnt_counter = 0;

UI::UI(LcdDisplay& display, RotaryEncoder& encoder, Buzzer& buzzer)
    : _display(display), _encoder(encoder), _buzzer(buzzer), _renderer(display)
{}

void UI::init()
{
    _renderer.init(); // clears buffer and loads custom chars
}

void UI::update()
{
    int pos = _encoder.getPosition();
    bool buttonPressed = _encoder.buttonPressed();

    _renderer.clearBuffer();

    // Draw "Position: <number>"
    std::string posStr = "Position: " + std::to_string(pos);
    Glyph posGlyphs[20]; // max 20 chars per row
    for (size_t i = 0; i < posStr.size() && i < 20; i++) {
        posGlyphs[i].id = posStr[i]; // normal ASCII
    }
    posGlyphs[posStr.size()].id = TERMINATOR.id; // terminator
    _renderer.drawGlyphsToBuffer(0, 0, posGlyphs);

    if (buttonPressed) {
        _buzzer.beep(1000, 100);
        bnt_counter++;
    }

    // Draw "Button Pressed: <number>"
    std::string btnStr = "Button Pressed: " + std::to_string(bnt_counter);
    Glyph btnGlyphs[20];
    for (size_t i = 0; i < btnStr.size() && i < 20; i++) {
        btnGlyphs[i].id = btnStr[i];
    }

    btnGlyphs[btnStr.size()].id = TERMINATOR.id; // terminator
    _renderer.drawGlyphsToBuffer(0, 1, btnGlyphs);

    // Custom char test

    Glyph customGlyphs[9];
    customGlyphs[0].id = CURSOR_L;
    customGlyphs[1].id = CURSOR_R;
    customGlyphs[2].id = CURSOR_DWN_L;
    customGlyphs[3].id = CURSOR_DWN_R;
    customGlyphs[4].id = BOX_CHECKD;
    customGlyphs[5].id = BOX_EMPTY;
    customGlyphs[6].id = ENTER;
    customGlyphs[7].id = PROGRESS_FULL;
    customGlyphs[8].id = TERMINATOR.id; // terminator
    _renderer.drawGlyphsToBuffer(0, 2, customGlyphs);

    _renderer.renderToDisplay();

    delay(100);
}

InputState UI::readInputs()
{
    InputState state;
    state.encoderDelta = _encoder.getPosition();
    state.buttonPressed = _encoder.buttonPressed();
    _encoder.reset();
    return state;
}