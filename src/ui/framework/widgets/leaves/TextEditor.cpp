#include "TextEditor.hpp"

namespace ui {
namespace widgets {

Size TextEditor::measure() const
{
    auto [leftDecorator, rightDecorator] = getDecorators();
    uint16_t width = _text.size() + leftDecorator.size() + rightDecorator.size();
    if (_isEditing) {
        width += 1; // account for the next symbol being rendered while editing
    }
    return {
        width,
        1
    };
}

void TextEditor::render(Renderer& r, Box canvasBox)
{
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return;

    auto [leftDecorator, rightDecorator] = getDecorators();

    int16_t x = canvasBox.x;
    int16_t y = canvasBox.y;

    uint16_t decorationWidth = leftDecorator.size() + rightDecorator.size();

    GlyphString textToRender = getTextToRender(canvasBox, decorationWidth);

    r.drawGlyphsToBuffer(x, y, leftDecorator);
    x += leftDecorator.size();

    if (textToRender.size() > 0)
    {
        r.drawGlyphsToBuffer(x, y, textToRender);
        x += static_cast<uint16_t>(textToRender.size());
    }

    r.drawGlyphsToBuffer(x, y, rightDecorator);
}

void TextEditor::handleInput(InputState& input) {
    if (!isFocused()) return;

    _isPressed = input.buttonDown;

    if (input.buttonPressed)
    {
        if (!_isEditing)
        {
            _isEditing = true;
        }
        else
        {
            if (getNextSymbol() == customChars::TextEditorEnter)
            {
                _isEditing = false;
                if (_onChange) _onChange(_text);
            }
            else if (getNextSymbol() == customChars::TextEditorDelete)
            {
                if (!_text.empty())
                    _text.pop_back();
            }
            else
            {
                if (_text.size() < _charCycle.size()) // prevent casting action glyphs to chars and appending them
                    _text += getNextSymbol().code;
            }
        }

        input.buttonPressed = false; // consume the button press event
    }
    if (_isEditing && input.encoderDelta != 0)
    {
        cycleNextSymbol(input.encoderDelta);
        input.encoderDelta = 0; // consume the encoder event
    }
}

GlyphString TextEditor::getTextToRender(Box canvasBox, uint16_t decorationWidth) const {
    uint16_t availableTextWidth = canvasBox.w - decorationWidth;

    GlyphString text = GlyphString(_text);
    if (_isEditing) text += getNextSymbol();

    GlyphString textToRender;
    if (text.size() > availableTextWidth)
    {
        if (_isEditing) {
            // If editing, show the end of the text with a leading ellipsis, ensuring the next symbol is visible
            textToRender = GlyphString(customChars::Ellipsis) + text.substr(text.size() - availableTextWidth + 1);
        }
        else
        {
            // If text exceeds available width, show the beginning of the text with a trailing ellipsis
            textToRender = text.substr(0, availableTextWidth - 1) + GlyphString(customChars::Ellipsis);
        }
    }
    else
    {
        textToRender = text;
    }

    return textToRender;
}

Glyph TextEditor::getNextSymbol() const {
    if (_currentCharIndex < _charCycle.size())
        return _charCycle[_currentCharIndex];
    else
        return _actionCycle[_currentCharIndex - _charCycle.size()];
}

void TextEditor::cycleNextSymbol(int delta) {
    int totalSymbols = static_cast<int>(_charCycle.size() + _actionCycle.size());
    _currentCharIndex = (_currentCharIndex + delta) % totalSymbols;
    if (_currentCharIndex < 0)
        _currentCharIndex += totalSymbols; // ensure positive index
}

std::pair<GlyphString, GlyphString> TextEditor::getDecorators() const {
    if (_isPressed)
        return std::make_pair(_style.leftPressed.getGlyphs(), _style.rightPressed.getGlyphs());
    else if (_isEditing)
        return std::make_pair(_style.leftEditing.getGlyphs(), _style.rightEditing.getGlyphs());
    else if (isFocused())
        return std::make_pair(_style.leftFocused.getGlyphs(), _style.rightFocused.getGlyphs());
    else
        return std::make_pair(_style.leftNormal.getGlyphs(), _style.rightNormal.getGlyphs());
}

} // namespace widgets
} // namespace ui