#ifndef LABEL_WIDGET_HPP
#define LABEL_WIDGET_HPP

#include "../widgetSystem/Widget.hpp"
#include "../text/TextSource.hpp"

enum class TextAlign
{
    Left,
    Center,
    Right
};


class LabelWidget : public Widget
{
public:
    LabelWidget(Rect box, TextSource& text, TextAlign align = TextAlign::Left);

    // Render the label within the given canvas
    void render(Renderer& r, Rect canvasBox) override;

    // Return the preferred size of the label
    Size measure() const override;

private:
    TextSource& text;
    TextAlign align;
};

#endif