#pragma once

#include <functional>
#include <sstream>
#include <type_traits>

#include "../core/ISelectable.hpp"
#include "../core/Widget.hpp"
#include "../../text/GlyphStringProvider.hpp"

namespace ui {
namespace widgets {

struct ValueSelectorStyle
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

template <typename T>
struct ValueSelectorProps
{
    T initialValue                                = {};
    std::function<T(T)> next                      = nullptr;
    std::function<T(T)> prev                      = nullptr;
    std::function<void(const T&)> onChange        = nullptr;
    std::function<std::string(const T&)> toString = nullptr;
    ValueSelectorStyle style                      = {};
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

    ValueSelector(ValueSelectorProps<T> props)
        : _value(std::move(props.initialValue))
        , _next(std::move(props.next))
        , _prev(std::move(props.prev))
        , _onChange(std::move(props.onChange))
        , _toString(props.toString ? std::move(props.toString) : defaultToString)
        , _style(std::move(props.style))
    {}

    ISelectable* tryGetSelectable() override { return this; }

    const T& value() const { return _value; }

    Size measure() const override
    {
        std::string text = _toString(_value);
        int width = static_cast<int>(text.size());

        if (_isEditing)
            width += _style.leftEditing.size() + _style.rightEditing.size();
        else if (_isPressed)
            width += _style.leftPressed.size() + _style.rightPressed.size();
        else if (isFocused())
            width += _style.leftFocused.size() + _style.rightFocused.size();
        else
            width += _style.leftNormal.size() + _style.rightNormal.size();

        return {
            static_cast<uint16_t>(width),
            1
        };
    }

    void render(Renderer& r, Box canvasBox) override
    {
        if (canvasBox.w == 0 || canvasBox.h == 0)
            return;

        GlyphString leftDecorator = {};
        GlyphString rightDecorator = {};

        if (_isPressed)
        {
            leftDecorator = _style.leftPressed.getGlyphs();
            rightDecorator = _style.rightPressed.getGlyphs();
        }
        else if (_isEditing)
        {
            leftDecorator = _style.leftEditing.getGlyphs();
            rightDecorator = _style.rightEditing.getGlyphs();
        }
        else if (isFocused())
        {
            leftDecorator = _style.leftFocused.getGlyphs();
            rightDecorator = _style.rightFocused.getGlyphs();
        }
        else
        {
            leftDecorator = _style.leftNormal.getGlyphs();
            rightDecorator = _style.rightNormal.getGlyphs();
        }

        uint16_t x = canvasBox.x;
        uint16_t y = canvasBox.y;

        std::string text = _toString(_value);

        uint16_t decorationWidth = leftDecorator.size() + rightDecorator.size();

        int textWidth = static_cast<int>(canvasBox.w) - static_cast<int>(decorationWidth);

        r.drawGlyphsToBuffer(x, y, leftDecorator);
        x += leftDecorator.size();

        if (textWidth > 0)
        {
            if (static_cast<int>(text.size()) > textWidth)
                text.resize(textWidth);

            r.drawTextToBuffer(x, y, text.c_str());
            x += static_cast<uint16_t>(text.size());
        }

        r.drawGlyphsToBuffer(x, y, rightDecorator);
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
    std::function<std::string(const T&)> _toString;

    bool _isEditing = false;
    bool _isPressed = false;

    ValueSelectorStyle _style;

    void updateValueByEncoderDelta(int delta)
    {
        if (delta > 0 && _next)
            _value = _next(_value);
        else if (delta < 0 && _prev)
            _value = _prev(_value);
        if (delta != 0 && _onChange)
            _onChange(_value);
    }
};

} // namespace widgets
} // namespace ui
