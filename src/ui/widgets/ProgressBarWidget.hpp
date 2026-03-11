#ifndef PROGRESS_BAR_WIDGET_HPP
#define PROGRESS_BAR_WIDGET_HPP

#include "../widgetSystem/Widget.hpp"

class ProgressBarWidget : public Widget
{
public:
    // Construct a progress bar with specified width and height
    // progress: 0-100 representing completion percentage
    ProgressBarWidget(Rect box,
                      uint8_t progress = 0,
                      Alignment align = {HorizontalAlignment::Left, VerticalAlignment::Top});

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

    // Update progress (0-100)
    void setProgress(uint8_t progress);
    uint8_t getProgress() const;

private:
    uint8_t _progress;  // 0-100
};

#endif
