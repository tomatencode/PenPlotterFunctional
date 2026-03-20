#pragma once

#include <cstdio>
#include <functional>
#include <limits>
#include <string>
#include <type_traits>

#include "../core/SelectableWidget.hpp"
#include "../core/Widget.hpp"

namespace ui {
namespace widgets {

struct ValueSelectorStyle
{
    Glyph leftNormal   = '(';
    Glyph rightNormal  = ')';
    Glyph leftFocused  = '<';
    Glyph rightFocused = '>';
    Glyph leftPressed  = '-';
    Glyph rightPressed = '-';
    Glyph leftEditing  = '>';
    Glyph rightEditing = '<';

    ValueSelectorStyle() = default;
    ValueSelectorStyle(Glyph ln, Glyph rn, Glyph lf, Glyph rf, Glyph lp, Glyph rp, Glyph le, Glyph re)
        : leftNormal(ln), rightNormal(rn), leftFocused(lf), rightFocused(rf),
          leftPressed(lp), rightPressed(rp), leftEditing(le), rightEditing(re) {}
};

// ValueSelector provides a focusable widget for +/− selection using rotary encoder and button.
// Typical styles:
// not focused -> (3)
// focused -> <3>
// pressed -> -3-
// focused+editing -> >3<

// Formatter helper, avoids invalid type_traits instantiation for non-enum when T is integral/float.
template <typename U, typename Enable = void>
struct ValueSelectorFormatter
{
    static std::string format(const U&) { return std::string(); }
};

template <typename U>
struct ValueSelectorFormatter<U, typename std::enable_if<std::is_enum<U>::value>::type>
{
    static std::string format(const U& value)
    {
        using Underlying = typename std::underlying_type<U>::type;
        return std::to_string(static_cast<Underlying>(value));
    }
};

template <typename U>
struct ValueSelectorFormatter<U, typename std::enable_if<std::is_integral<U>::value>::type>
{
    static std::string format(const U& value)
    {
        return std::to_string(value);
    }
};

template <typename U>
struct ValueSelectorFormatter<U, typename std::enable_if<std::is_floating_point<U>::value>::type>
{
    static std::string format(const U& value)
    {
        char buffer[32];
        std::snprintf(buffer, sizeof(buffer), "%.3g", static_cast<double>(value));
        return std::string(buffer);
    }
};

// Clamping helper
template <typename U, typename Enable = void>
struct ValueSelectorClamper
{
    static void clamp(U&, const U&, const U&) {}
};

template <typename U>
struct ValueSelectorClamper<U, typename std::enable_if<std::is_enum<U>::value>::type>
{
    static void clamp(U& value, const U& minValue, const U& maxValue)
    {
        using Underlying = typename std::underlying_type<U>::type;
        auto v = static_cast<Underlying>(value);
        auto mn = static_cast<Underlying>(minValue);
        auto mx = static_cast<Underlying>(maxValue);
        if (v < mn) v = mn;
        if (v > mx) v = mx;
        value = static_cast<U>(v);
    }
};

template <typename U>
struct ValueSelectorClamper<U, typename std::enable_if<std::is_arithmetic<U>::value>::type>
{
    static void clamp(U& value, const U& minValue, const U& maxValue)
    {
        if (value < minValue) value = minValue;
        if (value > maxValue) value = maxValue;
    }
};

// Encoder update helper
template <typename U, typename Enable = void>
struct ValueSelectorUpdater
{
    static void update(U&, const U&, const U&, const U&, int) {}
};

template <typename U>
struct ValueSelectorUpdater<U, typename std::enable_if<std::is_enum<U>::value>::type>
{
    static void update(U& value, const U& step, const U& minValue, const U& maxValue, int delta)
    {
        using Underlying = typename std::underlying_type<U>::type;
        auto v = static_cast<Underlying>(value);
        auto s = static_cast<Underlying>(step);
        v += static_cast<Underlying>(delta) * s;
        auto mn = static_cast<Underlying>(minValue);
        auto mx = static_cast<Underlying>(maxValue);
        if (v < mn) v = mn;
        if (v > mx) v = mx;
        value = static_cast<U>(v);
    }
};

template <typename U>
struct ValueSelectorUpdater<U, typename std::enable_if<std::is_arithmetic<U>::value>::type>
{
    static void update(U& value, const U& step, const U& minValue, const U& maxValue, int delta)
    {
        value += static_cast<U>(delta) * step;
        if (value < minValue) value = minValue;
        if (value > maxValue) value = maxValue;
    }
};

template <typename T>
class ValueSelector : public SelectableWidget
{
public:
    using Formatter = std::function<std::string(const T&)>;
    using ChangeCallback = std::function<void(const T&)>;

    ValueSelector(
        T initialValue,
        T minValue,
        T maxValue,
        T stepValue = static_cast<T>(1),
        ValueSelectorStyle style = ValueSelectorStyle{},
        Formatter formatter = nullptr,
        ChangeCallback onChange = nullptr
    )
        : _value(initialValue)
        , _minValue(minValue)
        , _maxValue(maxValue)
        , _step(stepValue)
        , _style(style)
        , _formatter(formatter ? formatter : defaultFormatter)
        , _onChange(onChange)
    {
        clampValue();
    }

    void setValue(const T& value)
    {
        _value = value;
        clampValue();
        invokeCallback();
    }

    const T& value() const { return _value; }

    void setEditing(bool editing) { _isEditing = editing; }
    bool isEditing() const { return _isEditing; }

    Size measure() const override
    {
        std::string text = formatValue();
        int width = static_cast<int>(text.size());

        if (_isEditing)
            width += ((_style.leftEditing.code != GLYPH_NONE.code) ? 1 : 0) + ((_style.rightEditing.code != GLYPH_NONE.code) ? 1 : 0);
        else if (_isPressed)
            width += ((_style.leftPressed.code != GLYPH_NONE.code) ? 1 : 0) + ((_style.rightPressed.code != GLYPH_NONE.code) ? 1 : 0);
        else if (isFocused())
            width += ((_style.leftFocused.code != GLYPH_NONE.code) ? 1 : 0) + ((_style.rightFocused.code != GLYPH_NONE.code) ? 1 : 0);
        else
            width += ((_style.leftNormal.code != GLYPH_NONE.code) ? 1 : 0) + ((_style.rightNormal.code != GLYPH_NONE.code) ? 1 : 0);

        return { static_cast<uint8_t>(width), 1 };
    }

    void render(Renderer& r, Rect canvasBox) override
    {
        if (canvasBox.w == 0 || canvasBox.h == 0)
            return;

        Glyph left = GLYPH_NONE;
        Glyph right = GLYPH_NONE;

        if (_isEditing)
        {
            left = _style.leftEditing;
            right = _style.rightEditing;
        }
        else if (_isPressed)
        {
            left = _style.leftPressed;
            right = _style.rightPressed;
        }
        else if (isFocused())
        {
            left = _style.leftFocused;
            right = _style.rightFocused;
        }
        else
        {
            left = _style.leftNormal;
            right = _style.rightNormal;
        }

        int x = canvasBox.x;
        int y = canvasBox.y;

        if (left.code != GLYPH_NONE.code && x < canvasBox.x + canvasBox.w)
        {
            r.drawGlyphToBuffer(x, y, left);
            x++;
        }

        std::string text = formatValue();
        int remaining = canvasBox.w - (x - canvasBox.x);
        if (right.code != GLYPH_NONE.code && remaining > 0)
            remaining--; // leave space for right marker

        if (remaining > 0)
        {
            if (static_cast<int>(text.size()) > remaining)
                text.resize(remaining);

            r.drawTextToBuffer(x, y, text.c_str());
            x += static_cast<int>(text.size());
        }

        if (right.code != GLYPH_NONE.code && x < canvasBox.x + canvasBox.w)
        {
            r.drawGlyphToBuffer(x, y, right);
        }
    }

    void handleInput(InputState& input) override
    {
        if (!isFocused())
            return;

        if (input.buttonState.buttonPressed)
        {
            // Toggle edit mode on button press
            _isEditing = !_isEditing;

            input.buttonState.buttonPressed = false;
            if (!_isEditing)
                _isPressed = false;
        }

        _isPressed = input.buttonState.buttonDown;

        if (_isEditing && input.encoderDelta != 0)
        {
            updateValueByEncoder(input.encoderDelta);
            input.encoderDelta = 0; // consume rotation while editing
        }

        // Keep buttonReleased consumed so parent does not also act on it
        if (input.buttonState.buttonReleased)
            input.buttonState.buttonReleased = false;
    }

private:
    T _value;
    T _minValue;
    T _maxValue;
    T _step;
    bool _isEditing = false;
    bool _isPressed = false;

    ValueSelectorStyle _style;
    Formatter _formatter;
    ChangeCallback _onChange;

    static std::string defaultFormatter(const T& value)
    {
        return ValueSelectorFormatter<T>::format(value);
    }

    std::string formatValue() const
    {
        if (_formatter)
            return _formatter(_value);
        return defaultFormatter(_value);
    }

    void clampValue()
    {
        ValueSelectorClamper<T>::clamp(_value, _minValue, _maxValue);
    }

    void updateValueByEncoder(int delta)
    {
        if (delta == 0)
            return;

        ValueSelectorUpdater<T>::update(_value, _step, _minValue, _maxValue, delta);
        invokeCallback();
    }

    void invokeCallback()
    {
        if (_onChange)
            _onChange(_value);
    }
};

} // namespace widgets
} // namespace ui
