#include "SpacerWidget.hpp"

namespace ui {
namespace widgets {

Spacer::Spacer(uint8_t width, uint8_t height) : _width(width), _height(height) {}

Size Spacer::measure() const {
    return Size{_width, _height};
}

void Spacer::render(Renderer& r, Rect canvasBox) {
    // No-op: just occupies space
}

} // namespace widgets
} // namespace ui