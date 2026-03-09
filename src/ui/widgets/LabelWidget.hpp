#ifndef LABEL_WIDGET_HPP
#define LABEL_WIDGET_HPP

#include "../widgetSystem/Widget.hpp"

class LabelWidget : public Widget
{
public:
    LabelWidget(Rect box, const char* text);

    void render(Renderer& r, Rect canvasBox) override;

private:
    const char* text;
};

#endif