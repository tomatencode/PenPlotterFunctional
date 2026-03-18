#pragma once

#include "../framework/widgets/core/Widget.hpp"

class WifiIndicator: public ui::widgets::Widget
{
public:
    WifiIndicator();

    void render(ui::Renderer& r, ui::widgets::Rect canvasBox) override;

    ui::widgets::Size measure() const override;
};