#pragma once

#include <functional>
#include <sstream>
#include <type_traits>

#include "../core/ISelectable.hpp"
#include "../core/Widget.hpp"

namespace ui {
namespace widgets {

struct ValueSelectorStyle
{
    Glyph leftNormal   = GLYPH_NONE;
    Glyph rightNormal  = GLYPH_NONE;
    Glyph leftFocused  = '<';
    Glyph rightFocused = '>';
    Glyph leftPressed  = '-';
    Glyph rightPressed = '-';
    Glyph leftEditing  = '>';
    Glyph rightEditing = '<';
};

// ValueSelector provides a focusable widget for +/− selection using rotary encoder and button.
// Typical styles:
// not focused -> 3
// focused -> <3>
// pressed -> -3-
// focused+editing -> >3<

template <typename T>
class ValueSelector : public Widget, public ISelectable
{
public:
    static std::string defaultToString(const T& value)
    {
        std::ostringstream ss;
        ss << value;
        return ss.str();
    }

    ValueSelector(
        T initialValue,
        std::function<T(T)> next,
        std::function<T(T)> prev,
        std::function<void(const T&)> onChange = nullptr,
        std::function<std::string(const T&)> ToString = nullptr,
        ValueSelectorStyle style = ValueSelectorStyle{}
    )
        : _value(initialValue)
        , _next(std::move(next))
        , _prev(std::move(prev))
        , _onChange(std::move(onChange))
        , _ToString(ToString ? std::move(ToString) : defaultToString)
        , _style(style)
    {}

    ISelectable* tryGetSelectable() override { return this; }

    const T& value() const { return _value; }

    Size measure() const override
    {
        std::string text = _ToString(_value);
        int width = static_cast<int>(text.size());

        if (_isEditing)
            width += ((_style.leftEditing.code != GLYPH_NONE.code) ? 1 : 0) + ((_style.rightEditing.code != GLYPH_NONE.code) ? 1 : 0);
        else if (_isPressed)
            width += ((_style.leftPressed.code != GLYPH_NONE.code) ? 1 : 0) + ((_style.rightPressed.code != GLYPH_NONE.code) ? 1 : 0);
        else if (isFocused())
            width += ((_style.leftFocused.code != GLYPH_NONE.code) ? 1 : 0) + ((_style.rightFocused.code != GLYPH_NONE.code) ? 1 : 0);
        else
            width += ((_style.leftNormal.code != GLYPH_NONE.code) ? 1 : 0) + ((_style.rightNormal.code != GLYPH_NONE.code) ? 1 : 0);

        return { width, 1 };
    }

    void render(Renderer& r, Rect canvasBox) override
    {
        if (canvasBox.w == 0 || canvasBox.h == 0)
            return;

        Glyph leftDecorator = GLYPH_NONE;
        Glyph rightDecorator = GLYPH_NONE;

        if (_isPressed)
        {
            leftDecorator = _style.leftPressed;
            rightDecorator = _style.rightPressed;
        }
        else if (_isEditing)
        {
            leftDecorator = _style.leftEditing;
            rightDecorator = _style.rightEditing;
        }
        else if (isFocused())
        {
            leftDecorator = _style.leftFocused;
            rightDecorator = _style.rightFocused;
        }
        else
        {
            leftDecorator = _style.leftNormal;
            rightDecorator = _style.rightNormal;
        }

        uint16_t x = canvasBox.x;
        uint16_t y = canvasBox.y;

        std::string text = _ToString(_value);

        uint16_t decorationWidth = 0;
        if (leftDecorator.code != GLYPH_NONE.code) decorationWidth++;
        if (rightDecorator.code != GLYPH_NONE.code) decorationWidth++;

        int textWidth = static_cast<int>(canvasBox.w) - static_cast<int>(decorationWidth);

        if (leftDecorator.code != GLYPH_NONE.code && canvasBox.w > 0)
        {
            r.drawGlyphToBuffer(x, y, leftDecorator);
            x++;
        }

        if (textWidth > 0)
        {
            if (static_cast<int>(text.size()) > textWidth)
                text.resize(textWidth);

            r.drawTextToBuffer(x, y, text.c_str());
            x += static_cast<uint16_t>(text.size());
        }

        if (rightDecorator.code != GLYPH_NONE.code && canvasBox.w >= decorationWidth)
        {
            r.drawGlyphToBuffer(x, y, rightDecorator);
        }
    }

    void handleInput(InputState& input) override
    {
        if (!isFocused()) return;

        _isPressed = input.buttonDown;

        if (input.buttonPressed)
        {
            // Toggle edit mode on button press
            _isEditing = !_isEditing;

            input.buttonPressed = false; // consume the event so parent does not also act on it
        }

        if (_isEditing && input.encoderDelta != 0)
        {
            updateValueByEncoderDelta(input.encoderDelta);
            input.encoderDelta = 0; // consume rotation while editing
        }
    }

    void reload() override {
        ISelectable::reload();
        _isEditing = false;
        _isPressed = false;
    }

private:
    T _value;
    std::function<T(T)> _next;
    std::function<T(T)> _prev;
    std::function<void(const T&)> _onChange;
    std::function<std::string(const T&)> _ToString;

    bool _isEditing = false;
    bool _isPressed = false;

    ValueSelectorStyle _style;

    void updateValueByEncoderDelta(int delta)
    {
        if (delta > 0)
            _value = _next(_value);
        else if (delta < 0)
            _value = _prev(_value);

        if (delta != 0 && _onChange)
            _onChange(_value);
    }
};

} // namespace widgets
} // namespace ui
