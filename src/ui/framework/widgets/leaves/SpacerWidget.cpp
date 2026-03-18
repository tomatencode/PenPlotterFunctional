#include "SpacerWidget.hpp"

namespace ui {
namespace widgets {

SpacerWidget::SpacerWidget(uint8_t width, uint8_t height) : _width(width), _height(height) {}

Size SpacerWidget::measure() const {
    return Size{_width, _height};
}

void SpacerWidget::render(Renderer& r, Rect canvasBox) {
    // No-op: just occupies space
}

} // namespace widgets
} // namespace ui