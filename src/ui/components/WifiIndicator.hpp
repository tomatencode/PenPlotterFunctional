#pragma once

#include "../framework/widgets/core/Widget.hpp"

class WifiIndicator: public Widget
{
public:
    WifiIndicator();

    void render(Renderer& r, Rect canvasBox) override;

    Size measure() const override;
};