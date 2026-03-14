#include "ScrollableVerticalLayout.hpp"
#include "../core/SelectableWidget.hpp"
#include <cassert>

ScrollableVerticalLayout::ScrollableVerticalLayout(Widget* children[], size_t count, const LayoutStyle& style)
    : VerticalLayout(children, count, style)
{
    // Scrollable layouts don't support advanced spacing modes
    // as they don't make sense when content exceeds screen size
    if (style.spacingMode != SpacingMode::Fixed) {
        // Throw error - only Fixed spacing supported
        assert(false && "ScrollableVerticalLayout only supports Fixed spacing mode");
    }
}

void ScrollableVerticalLayout::updateScrollOffset(uint8_t visibleHeight)
{
    uint16_t childY = style().marginTop; // Start at top margin
    // Find the focused child and ensure it's visible
    const size_t count = childCount();

    for (size_t i = 0; i < count; i++)
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

                // Scroll up if child is above visible area
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
        childY += style().spacing;
    }
}

void ScrollableVerticalLayout::render(Renderer& r, Rect canvasBox)
{
    const size_t count = childCount();
    if (count == 0 || canvasBox.w == 0 || canvasBox.h == 0)
        return; // nothing to render

    // Apply margins to create content area
    Rect contentArea = {
        static_cast<uint8_t>(canvasBox.x + style().marginLeft),
        static_cast<uint8_t>(canvasBox.y + style().marginTop),
        static_cast<uint8_t>(canvasBox.w - style().marginLeft - style().marginRight),
        static_cast<uint8_t>(canvasBox.h - style().marginTop - style().marginBottom)
    };

    // Update scroll offset to keep focused child visible
    updateScrollOffset(contentArea.h);

    // Calculate starting Y position
    int currentY = contentArea.y - _scrollOffset;

    // Render each child
    for (size_t i = 0; i < count; i++)
    {
        Widget* childWidget = child(i);
        if (childWidget == nullptr)
            continue;

        Size childSize = childWidget->measure();

        // Calculate X position based on horizontal alignment
        int childX = contentArea.x;
        switch (style().horizontalAlign)
        {
            case HorizontalAlignment::Center:
                childX += (contentArea.w - childSize.w) / 2;
                break;
            case HorizontalAlignment::Right:
                childX += contentArea.w - childSize.w;
                break;
            default: // Left
                break;
        }

        // Create canvas for this child
        // VerticalLayout fills horizontal space, so give child full available width
        Rect childCanvas = {
            static_cast<uint8_t>(childX),
            static_cast<uint8_t>(currentY),
            static_cast<uint8_t>(contentArea.w - (childX - contentArea.x)),  // Full available width
            std::min(childSize.h, static_cast<uint8_t>(contentArea.y + contentArea.h - currentY)) // Don't exceed content area
        };

        // Only render if child is within visible area
        if (currentY + childSize.h > canvasBox.y && currentY < canvasBox.y + canvasBox.h)
        {
            childWidget->render(r, childCanvas);
        }

        // Move to next position
        currentY += childSize.h + style().spacing;
    }
}

Size ScrollableVerticalLayout::measure() const
{
    // ScrollableVerticalLayout uses the same measurement rules as VerticalLayout
    return VerticalLayout::measure();
}

