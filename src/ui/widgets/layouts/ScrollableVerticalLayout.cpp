#include "ScrollableVerticalLayout.hpp"
#include "../core/SelectableWidget.hpp"

ScrollableVerticalLayout::ScrollableVerticalLayout(Widget* children[], size_t count)
    : VerticalLayout(children, count), _scrollOffset(0)
{}

void ScrollableVerticalLayout::updateScrollOffset(uint8_t visibleHeight)
{
    uint16_t childY = 0;
    // Find the focused child and ensure it's visible
    for (size_t i = 0; i < childCount(); i++)
    {
        Widget* childWidget = child(i);
        if (childWidget == nullptr) continue;

        if (childWidget->isSelectable())
        {
            SelectableWidget* selectable = static_cast<SelectableWidget*>(childWidget);
            if (selectable->isFocused())
            {
                Size childSize = childWidget->measure();
                int16_t childBottom = childY + childSize.h;

                // Scroll up if child is above visible area (y < 0)
                if (childY < _scrollOffset)
                {
                    _scrollOffset = childY;  // Move child down into view
                }
                else if (childBottom > _scrollOffset + visibleHeight)
                {
                    _scrollOffset = childBottom - visibleHeight;  // Move child up into view
                }
                
                return;
            }
        }

        childY += childWidget->measure().h;
    }
}

void ScrollableVerticalLayout::render(Renderer& r, Rect canvasBox)
{
    // Update scroll to keep focused child visible
    updateScrollOffset(canvasBox.h);

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
