#pragma once

#include "Alignment.hpp"
#include <cstdint>

namespace ui {
namespace widgets {

enum class SpacingMode {
    Fixed,      // Fixed spacing between children
    Even,       // Distribute children evenly across available space
    SpaceBetween, // Space between children, no space at edges
    SpaceAround   // Space around each child including edges
};

struct LayoutStyle {
    SpacingMode spacingMode = SpacingMode::Fixed;
    uint8_t spacing = 0;  // Used for Fixed mode
    uint8_t marginLeft = 0;
    uint8_t marginRight = 0;
    uint8_t marginTop = 0;
    uint8_t marginBottom = 0;

    // Alignment - will be interpreted differently by horizontal vs vertical layouts
    VerticalAlignment verticalAlign = VerticalAlignment::Top;
    HorizontalAlignment horizontalAlign = HorizontalAlignment::Left;

    constexpr LayoutStyle() = default;
    
    constexpr LayoutStyle(SpacingMode mode, uint8_t space = 0, 
                         uint8_t mLeft = 0, uint8_t mRight = 0,
                         uint8_t mTop = 0, uint8_t mBottom = 0,
                         VerticalAlignment vAlign = VerticalAlignment::Top,
                         HorizontalAlignment hAlign = HorizontalAlignment::Left)
        : spacingMode(mode), spacing(space),
          marginLeft(mLeft), marginRight(mRight),
          marginTop(mTop), marginBottom(mBottom),
          verticalAlign(vAlign), horizontalAlign(hAlign) {}
};

} // namespace widgets
} // namespace ui
