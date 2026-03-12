#pragma once

#include "../widgetSystem/Widget.hpp"

class ProgressBarWidget : public Widget
{
public:
    // Construct a progress bar with specified width and height
    // getProgress: function pointer that returns progress 0-100
    ProgressBarWidget(uint8_t width,
                      uint8_t height,
                      uint8_t (*getProgress)() = nullptr);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

private:
    uint8_t (*_getProgress)();  // Function pointer returning 0-100
};