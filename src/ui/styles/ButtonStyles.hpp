#pragma once

#include "../framework/widgets/leaves/Button.hpp"

namespace ui {
namespace styles {

inline widgets::ButtonStyle listButtonStyle = {
    .leftNormal = GlyphString{},
    .rightNormal = GlyphString{},
    .leftFocused = GlyphString('>'),
    .rightFocused = GlyphString{},
    .leftPressed = GlyphString('-'),
    .rightPressed = GlyphString{}
};

} // namespace styles
} // namespace ui