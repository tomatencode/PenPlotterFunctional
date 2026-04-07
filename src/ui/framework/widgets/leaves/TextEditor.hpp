#pragma once

#include <functional>

#include "../core/ISelectable.hpp"
#include "../core/Widget.hpp"
#include "../../text/GlyphStringProvider.hpp"
#include "../../text/CustomChars.hpp"

namespace ui {
namespace widgets {

struct TextEditorStyle
{
    GlyphStringProvider leftNormal   = {};
    GlyphStringProvider rightNormal  = {};
    GlyphStringProvider leftFocused  = '<';
    GlyphStringProvider rightFocused = '>';
    GlyphStringProvider leftPressed  = '-';
    GlyphStringProvider rightPressed = '-';
    GlyphStringProvider leftEditing  = '>';
    GlyphStringProvider rightEditing = '<';
};

inline std::vector<char> PrintableAsciiCharCycle = {
    // Lowercase
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
    // Uppercase
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z',
    // Digits
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    // Separators
    ' ', '_', '-', '.',
    // Common special
    '@', '#', '$', '%', '&', '*', '!', '?',
    // Brackets / pairs
    '(', ')', '[', ']', '{', '}', '<', '>',
    // Math / comparison
    '+', '=', '/', '|', '^', '~',
    // Punctuation
    ',', ':', ';', '"', '`'
};

struct TextEditorProps
{
    std::string initialText = {};
    std::function<void(std::string)> onChange = nullptr;
    std::vector<char> charCycle = PrintableAsciiCharCycle;
    TextEditorStyle style = {};
};

class TextEditor : public Widget, public ISelectable
{
public:
    TextEditor(TextEditorProps props)
        : _style(std::move(props.style)),
          _text(std::move(props.initialText)),
          _charCycle(std::move(props.charCycle)),
          _currentCharIndex(0),
          _onChange(std::move(props.onChange))
    {}

    ISelectable* tryGetSelectable() override { return this; }

    const std::string text() const { return _text; }

    Size measure() const override;
    void render(Renderer& r, Box canvasBox) override;
    void handleInput(InputState& input) override;

    void reload() override {
        ISelectable::reload();
        _isEditing = false;
        _isPressed = false;
    }

private:
    std::string _text;
    int _currentCharIndex = 0;
    std::function<void(std::string)> _onChange;
    std::vector<char> _charCycle;

    std::vector<Glyph> _actionCycle = {
        customChars::TextEditorEnter,
        customChars::TextEditorDelete
    };

    bool _isEditing = false;
    bool _isPressed = false;

    TextEditorStyle _style;

    GlyphString getTextToRender(Box canvasBox, uint16_t decorationWidth) const;

    Glyph getNextSymbol() const;
    void cycleNextSymbol(int delta);

    std::pair<GlyphString, GlyphString> getDecorators() const;
};

} // namespace widgets
} // namespace ui