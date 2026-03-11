#pragma once

#include "Widget.hpp"
#include "Container.hpp"

class ScrollableContainer : public Container
{
public:
    ScrollableContainer(Rect box, Alignment align, Widget* children[], size_t count)
        : Container(box, align, children, count), _scrollOffset(0) {}

void render(Renderer& r, Rect canvasBox) override
    {
        updateScrollOffset(); // Calculate needed offset
        
        Rect scrolledCanvas = canvasBox;
        scrolledCanvas.y -= _scrollOffset; // Adjust canvas position
        Container::render(r, scrolledCanvas);
    }

private:
    void updateScrollOffset()
    {
        for (size_t i = 0; i < childCount(); i++)
        {
            Widget* c = child(i);
            if (c && c->isSelectable())
            {
                SelectableWidget* sw = static_cast<SelectableWidget*>(c);
                if (sw->isFocused())
                {
                    Rect widgetBox = c->box();
                    int8_t widgetBottom = widgetBox.y + widgetBox.h;
                    int8_t visibleBottom = _scrollOffset + box().h;
                    
                    // Scroll up if widget is above visible area
                    if (widgetBox.y < _scrollOffset)
                        _scrollOffset = widgetBox.y;
                    // Scroll down if widget is below visible area
                    else if (widgetBottom > visibleBottom)
                        _scrollOffset = widgetBottom - box().h;
                    return;
                }
            }
        }
    }

    int8_t _scrollOffset; // positive scrolls down, negative scrolls up
};