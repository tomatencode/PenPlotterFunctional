#include "ScrollableVerticalLayout.hpp"
#include "../core/SelectableWidget.hpp"

ScrollableVerticalLayout::ScrollableVerticalLayout(Widget* children[], size_t count)
    : VerticalLayout(children, count), _scrollOffset(0)
{}

int16_t ScrollableVerticalLayout::getChildY(size_t index) const
{
    // Calculate Y position relative to layout start, accounting for scroll
    int16_t y = -_scrollOffset;
    
    for (size_t i = 0; i < index; i++)
    {
        Widget* childWidget = child(i);
        if (childWidget)
            y += childWidget->measure().h;
    }
    
    return y;
}

void ScrollableVerticalLayout::updateScrollOffset()
{
    // Find the focused child and ensure it's visible
    for (size_t i = 0; i < childCount(); i++)
    {
        Widget* childWidget = child(i);
        if (childWidget == nullptr)
            continue;

        if (childWidget->isSelectable())
        {
            SelectableWidget* selectable = static_cast<SelectableWidget*>(childWidget);
            if (selectable->isFocused())
            {
                // Get child's position and height relative to scroll
                Size childSize = childWidget->measure();
                int16_t childY = getChildY(i);
                int16_t childBottom = childY + childSize.h;

                // Scroll up if child is above visible area (y < 0)
                if (childY < 0)
                {
                    _scrollOffset -= childY;  // Move child down into view
                }
                // Scroll down if child is below visible area
                // Assume layout is full height of canvasBox
                // We'll check this during render when we have canvasBox.h
                
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

    if (canvasBox.w == 0 || canvasBox.h == 0)
        return;

    int currentY = canvasBox.y - _scrollOffset;

    // Render each child stacked vertically with scroll offset applied
    for (size_t i = 0; i < childCount(); i++)
    {
        Widget* childWidget = child(i);
        if (childWidget == nullptr)
            continue;

        Size childSize = childWidget->measure();

        // Create a canvas for this child
        Rect childCanvas = {
            canvasBox.x,
            static_cast<uint8_t>(currentY),
            canvasBox.w,
            childSize.h
        };

        // Only render if child is within visible area
        if (currentY + childSize.h > canvasBox.y && currentY < canvasBox.y + canvasBox.h)
        {
            childWidget->render(r, childCanvas);
        }

        currentY += childSize.h;
    }
}
