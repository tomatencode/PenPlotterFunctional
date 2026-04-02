#pragma once

#include "../../input/InputState.hpp"

namespace ui {
namespace widgets {

class ISelectable
{
public:
    virtual ~ISelectable() = default;

    virtual void focus() = 0;
    virtual void unfocus() = 0;
    virtual bool isFocused() const = 0;

    virtual void handleInput(InputState& input) = 0;
};

} // namespace widgets
} // namespace ui
