#pragma once
#include "../core/Widget.hpp"

namespace ui {
namespace widgets {

class Spacer : public Widget {
public:
    Spacer(uint8_t width, uint8_t height);
    Size measure() const override;
    void render(Renderer& r, Rect canvasBox) override;  // Does nothing
private:
    uint8_t _width;
    uint8_t _height;
};

} // namespace widgets
} // namespace ui
