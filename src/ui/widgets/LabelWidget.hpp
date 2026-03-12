#pragma once

#include "../widgetSystem/Widget.hpp"
#include "../text/TextSource.hpp"

class LabelWidget : public Widget
{
public:
    LabelWidget(TextSource& text);

    // Render the label within the given canvas
    void render(Renderer& r, Rect canvasBox) override;

    // Return the preferred size of the label
    Size measure() const override;

private:
    TextSource& text;
};