#include "ScrollableVerticalLayout.hpp"
#include "../core/SelectableWidget.hpp"
#include <cassert>

namespace ui {
namespace widgets {

ScrollableVerticalLayout::ScrollableVerticalLayout(const ScrollableVerticalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children)
    : Layout(std::move(children)), _style(style)
{
}

void ScrollableVerticalLayout::updateScrollOffset(uint8_t visibleHeight)
{
    uint16_t childY = _style.marginTop; // Start at top margin
    // Find the focused child and ensure it's visible
    const size_t count = EnChildCount();

    for (size_t i = 0; i < count; i++)
    {
        Widget* childWidget = EnChild(i);
        if (childWidget == nullptr) continue;

        if (childWidget->isSelectable())
        {
            Selectable* selectable = static_cast<Selectable*>(childWidget);
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
        childY += _style.spacing;
    }
}

void ScrollableVerticalLayout::render(Renderer& r, Rect canvasBox)
{
    const size_t count = EnChildCount();
    if (count == 0 || canvasBox.w == 0 || canvasBox.h == 0)
        return; // nothing to render

    // Apply margins to create content area
    Rect contentArea = {
        static_cast<uint8_t>(canvasBox.x + _style.marginLeft),
        static_cast<uint8_t>(canvasBox.y + _style.marginTop),
        static_cast<uint8_t>(canvasBox.w - _style.marginLeft - _style.marginRight),
        static_cast<uint8_t>(canvasBox.h - _style.marginTop - _style.marginBottom)
    };

    // Update scroll offset to keep focused child visible
    updateScrollOffset(contentArea.h);

    // Calculate starting Y position
    int currentY = contentArea.y - _scrollOffset;

    // Render each child
    for (size_t i = 0; i < count; i++)
    {
        Widget* childWidget = EnChild(i);
        if (childWidget == nullptr)
            continue;

        Size minChildSize = childWidget->measure();
        bool canExpandHorizontally = childWidget->canExpandHorizontally();

        Size desiredChildSize = minChildSize;
        if (canExpandHorizontally)
            desiredChildSize.w = contentArea.w; // Fill available width

        Size childSize = {std::min(desiredChildSize.w, static_cast<uint8_t>(contentArea.w)),
                          desiredChildSize.h}; // ScrollableVertical layout does not constrain height, only width

        // Calculate X position based on horizontal alignment
        int childX = contentArea.x;
        switch (_style.horizontalAlign)
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
            childSize.w,
            childSize.h
        };

        // Only render if child is within visible area
        if (currentY + childSize.h > canvasBox.y && currentY < canvasBox.y + canvasBox.h)
        {
            childWidget->render(r, childCanvas);
        }

        // Move to next position
        currentY += childSize.h + _style.spacing;
    }
}

Size ScrollableVerticalLayout::measure() const
{
    const size_t count = EnChildCount();
    if (count == 0) return Size{0, 0};
    
    // Calculate content size (excluding margins)
    uint16_t contentHeight = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = EnChild(i))
            contentHeight += w->measure().h;
    }
    
    // Add spacing
    contentHeight += (count - 1) * _style.spacing;

    
    uint16_t contentWidth = 0;
    
    // Find maximum width among children
    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = EnChild(i))
        {
            Size childSize = w->measure();
            if (childSize.w > contentWidth)
                contentWidth = childSize.w;
        }
    }
    
    // Add margins to get total size
    uint16_t totalWidth = contentWidth + _style.marginLeft + _style.marginRight;
    uint16_t totalHeight = contentHeight + _style.marginTop + _style.marginBottom;
    
    return Size{static_cast<uint8_t>(totalWidth), static_cast<uint8_t>(totalHeight)};
}

bool ScrollableVerticalLayout::canExpandHorizontally() const {
    // ScrollableVerticalLayout can expand horizontally if any child can expand horizontally
    for (size_t i = 0; i < EnChildCount(); i++)
    {
        if (Widget* w = EnChild(i))
        {
            if (w->canExpandHorizontally())
                return true;
        }
    }
    return false;
}

bool ScrollableVerticalLayout::canExpandVertically() const {
    // ScrollableVerticalLayout does not expand vertically because it scrolls instead
    return false;
}

} // namespace widgets
} // namespace ui