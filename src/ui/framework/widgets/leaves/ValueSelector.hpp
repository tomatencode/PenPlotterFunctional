#pragma once

#include <functional>
#include <memory>
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
    std::function<T()> getValue                   = nullptr;  // source of truth, called on every render
    std::function<void(T)> next                   = nullptr;  // apply increment; new value read back via getValue
    std::function<void(T)> prev                   = nullptr;  // apply decrement
    std::function<GlyphString(const T&)> toString = nullptr;
    ValueSelectorStyle style                      = {};

    // Special case: widget owns its own copy of the value (no external source of truth).
    // Use for purely local UI state that does not need to persist or sync externally.
    static ValueSelectorProps<T> withLocalState(
        T initialValue,
        std::function<T(T)> next,
        std::function<T(T)> prev,
        std::function<void(const T&)> onChange = nullptr,
        std::function<GlyphString(const T&)> toString = nullptr,
        ValueSelectorStyle style = {})
    {
        auto state = std::make_shared<T>(std::move(initialValue));
        return ValueSelectorProps<T>{
            .getValue = [state]() -> T { return *state; },
            .next = [state, next, onChange](T current) {
                *state = next(current);
                if (onChange) onChange(*state);
            },
            .prev = [state, prev, onChange](T current) {
                *state = prev(current);
                if (onChange) onChange(*state);
            },
            .toString = std::move(toString),
            .style    = std::move(style),
        };
    }
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
        : _getValue(std::move(props.getValue))
        , _next(std::move(props.next))
        , _prev(std::move(props.prev))
        , _toString(props.toString ? std::move(props.toString) : defaultToString)
        , _style(std::move(props.style))
    {}

    ISelectable* tryGetSelectable() override { return this; }

    T value() const { return _getValue(); }

    Size measure() const override
    {
        GlyphString text = _toString(_getValue());

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

        GlyphString text = _toString(_getValue());

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
    std::function<T()> _getValue;
    std::function<void(T)> _next;
    std::function<void(T)> _prev;
    std::function<GlyphString(const T&)> _toString;

    bool _isEditing = false;
    bool _isPressed = false;

    ValueSelectorStyle _style;

    void updateValueByEncoderDelta(int delta)
    {
        const T current = _getValue();
        if (delta > 0 && _next)
            _next(current);
        else if (delta < 0 && _prev)
            _prev(current);
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
