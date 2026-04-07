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
    std::function<GlyphString(const T&)> toString = nullptr;
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
    static GlyphString defaultToString(const T& value)
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
        GlyphString text = _toString(_value);

        auto [leftDecorator, rightDecorator] = getDecorators();
        uint16_t width = text.size() + leftDecorator.size() + rightDecorator.size();

        return {
            width,
            1
        };
    }

    void render(Renderer& r, Box canvasBox) override
    {
        if (canvasBox.w == 0 || canvasBox.h == 0)
            return;

        auto [leftDecorator, rightDecorator] = getDecorators();

        int16_t x = canvasBox.x;
        int16_t y = canvasBox.y;

        GlyphString text = _toString(_value);

        uint16_t decorationWidth = leftDecorator.size() + rightDecorator.size();

        int textWidth = static_cast<int>(canvasBox.w) - static_cast<int>(decorationWidth);

        r.drawGlyphsToBuffer(x, y, leftDecorator);
        x += leftDecorator.size();

        if (textWidth > 0)
        {
            if (static_cast<int>(text.size()) > textWidth)
                text = text.substr(0, textWidth);

            r.drawGlyphsToBuffer(x, y, text);
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
    std::function<GlyphString(const T&)> _toString;

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

    std::pair<GlyphString, GlyphString> getDecorators() const {
        if (_isPressed)
            return std::make_pair(_style.leftPressed.getGlyphs(), _style.rightPressed.getGlyphs());
        else if (_isEditing)
            return std::make_pair(_style.leftEditing.getGlyphs(), _style.rightEditing.getGlyphs());
        else if (isFocused())
            return std::make_pair(_style.leftFocused.getGlyphs(), _style.rightFocused.getGlyphs());
        else
            return std::make_pair(_style.leftNormal.getGlyphs(), _style.rightNormal.getGlyphs());
    }
};

} // namespace widgets
} // namespace ui
