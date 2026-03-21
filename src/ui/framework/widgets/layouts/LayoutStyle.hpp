#pragma once

#include <cstdint>

namespace ui {
namespace widgets {

enum class SpacingMode {
    Fixed,      // Fixed spacing between children
    Even,       // Distribute children evenly across available space
    SpaceBetween, // Space between children, no space at edges
    SpaceAround   // Space around each child including edges
};

enum class HorizontalAlignment {
    Left,
    Center,
    Right
};

enum class VerticalAlignment {
    Top,
    Middle,
    Bottom
};

} // namespace widgets
} // namespace ui
