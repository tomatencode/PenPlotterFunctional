#pragma once

#include "../framework/widgets/leaves/Button.hpp"

namespace ui {
namespace styles {

inline widgets::ButtonStyle listButtonStyle = {
    .leftNormal = GLYPH_NONE,
    .rightNormal = GLYPH_NONE,
    .leftFocused = '>',
    .rightFocused = GLYPH_NONE,
    .leftPressed = '-',
    .rightPressed = GLYPH_NONE
};

} // namespace styles
} // namespace ui