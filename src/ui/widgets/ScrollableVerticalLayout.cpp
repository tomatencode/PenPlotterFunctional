#include "ScrollableVerticalLayout.hpp"
#include "../widgetSystem/SelectableWidget.hpp"
#include "../widgetSystem/WidgetUtils.hpp"

ScrollableVerticalLayout::ScrollableVerticalLayout(Rect box, Alignment align, Widget* children[], size_t count)
    : VerticalLayout(box, align, children, count), _scrollOffset(0)
{}

int16_t ScrollableVerticalLayout::getChildY(size_t index) const
{
    int16_t y = box().y - _scrollOffset;
    
    for (size_t i = 0; i < index; i++)
    {
        if (_children[i])
            y += _children[i]->measure().h;
    }
    
    return y;
}

void ScrollableVerticalLayout::updateScrollOffset()
{
    // Find the focused child
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i] == nullptr)
            continue;

        if (_children[i]->isSelectable())
        {
            SelectableWidget* selectable = static_cast<SelectableWidget*>(_children[i]);
            if (selectable->isFocused())
            {
                // Get child's position and height
                Size childSize = _children[i]->measure();
                int16_t childY = getChildY(i);
                int16_t childBottom = childY + childSize.h;

                int16_t containerBottom = box().y + box().h;

                // Scroll up if child is above visible area
                if (childY < box().y)
                {
                    _scrollOffset -= (box().y - childY);
                }
                // Scroll down if child is below visible area
                else if (childBottom > containerBottom)
                {
                    _scrollOffset += (childBottom - containerBottom);
                }

                return;
            }
        }
    }
}

void ScrollableVerticalLayout::ensureFocusedChildVisible()
{
    updateScrollOffset();
}

void ScrollableVerticalLayout::render(Renderer& r, Rect canvasBox)
{
    // Update scroll to keep focused child visible
    updateScrollOffset();

    // Compute final drawing rectangle including alignment + clipping
    Rect drawRect = computeContentAlignment(box(), align(), Size{box().w, box().h}, canvasBox);

    if (drawRect.w == 0 || drawRect.h == 0)
        return; // nothing visible

    // Render each child stacked vertically with scroll offset applied
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i] == nullptr)
            continue;

        Size childSize = _children[i]->measure();
        int16_t childY = getChildY(i);

        // Create a canvas for this child
        Rect childCanvas = {
            drawRect.x,
            static_cast<uint8_t>(childY),
            drawRect.w,
            childSize.h
        };

        // Only render if child is within visible area
        if (childY + childSize.h > drawRect.y && childY < drawRect.y + drawRect.h)
        {
            _children[i]->render(r, childCanvas);
        }
    }
}
