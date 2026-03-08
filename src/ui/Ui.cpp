#include "UI.hpp"
#include "render/CustomChars.hpp"
#include <string>

int bnt_prs_counter = 0;
int bnt_rel_counter = 0;

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

    // If button was pressed, increment counter and beep
    if (buttonPressed) {
        _buzzer.beep(1000, 100);
        bnt_prs_counter++;
    }

    // Draw "Button Pressed: <number>"
    std::string btnStr = "Button Pressed: " + std::to_string(bnt_prs_counter);
    Glyph btnGlyphs[20];
    for (size_t i = 0; i < btnStr.size() && i < 20; i++) {
        btnGlyphs[i].id = btnStr[i];
    }

    btnGlyphs[btnStr.size()].id = TERMINATOR.id; // terminator
    _renderer.drawGlyphsToBuffer(0, 1, btnGlyphs);

    // If button was released, increment counter
    if (_encoder.buttonReleased()) {
        bnt_rel_counter++;
    }

    // Draw "Button Released: <number>"
    std::string btnRelStr = "Button Released: " + std::to_string(bnt_rel_counter);
    Glyph btnRelGlyphs[20];
    for (size_t i = 0; i < btnRelStr.size() && i < 20; i++) {
        btnRelGlyphs[i].id = btnRelStr[i];
    }
    btnRelGlyphs[btnRelStr.size()].id = TERMINATOR.id; // terminator
    _renderer.drawGlyphsToBuffer(0, 2, btnRelGlyphs);

    // Draw "Button Down: <boolean>"
    std::string btnDownStr = "Button Down: " + std::to_string(_encoder.buttonDown());
    Glyph btnDownGlyphs[20];
    for (size_t i = 0; i < btnDownStr.size() && i < 20; i++) {
        btnDownGlyphs[i].id = btnDownStr[i];
    }
    btnDownGlyphs[btnDownStr.size()].id = TERMINATOR.id; // terminator
    _renderer.drawGlyphsToBuffer(0, 3, btnDownGlyphs);

    _renderer.renderToDisplay();

    delay(100);
}

InputState UI::readInputs()
{
    InputState state;
    state.encoderDelta = _encoder.getPosition();
    state.buttonPressed = _encoder.buttonPressed();
    state.buttonReleased = _encoder.buttonReleased();
    state.buttonDown = _encoder.buttonDown();
    _encoder.reset();
    return state;
}