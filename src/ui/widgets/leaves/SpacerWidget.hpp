#pragma once
#include "../core/Widget.hpp"

class SpacerWidget : public Widget {
public:
    SpacerWidget(uint8_t width, uint8_t height);
    Size measure() const override;
    void render(Renderer& r, Rect canvasBox) override;  // Does nothing
private:
    uint8_t _width;
    uint8_t _height;
};