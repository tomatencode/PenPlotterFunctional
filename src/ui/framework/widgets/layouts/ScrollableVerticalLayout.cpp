#include "ScrollableVerticalLayout.hpp"
#include "../core/SelectableWidget.hpp"
#include <cassert>

ScrollableVerticalLayout::ScrollableVerticalLayout(Widget* children[], size_t count, const LayoutStyle& style)
    : _count(0), _style(style)
{
    // Scrollable layouts don't support advanced spacing modes
    // as they don't make sense when content exceeds screen size
    if (style.spacingMode != SpacingMode::Fixed) {
        // Throw error - only Fixed spacing supported
        assert(false && "ScrollableVerticalLayout only supports Fixed spacing mode");
    }

    for (size_t i = 0; i < count && i < MAX_LAYOUT_CHILDREN; i++)
    {
        if (children[i] != nullptr)
            _children[_count++] = children[i];
    }
}

void ScrollableVerticalLayout::updateScrollOffset(uint8_t visibleHeight)
{
    uint16_t childY = _style.marginTop; // Start at top margin
    // Find the focused child and ensure it's visible
    for (size_t i = 0; i < _count; i++)
    {
        Widget* childWidget = _children[i];
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
        childY += _style.spacing;
    }
}

void ScrollableVerticalLayout::render(Renderer& r, Rect canvasBox)
{
    if (_count == 0 || canvasBox.w == 0 || canvasBox.h == 0)
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
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i] == nullptr)
            continue;

        Size childSize = _children[i]->measure();

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
            static_cast<uint8_t>(contentArea.w - childX),  // Full available width
            std::min(childSize.h, static_cast<uint8_t>(contentArea.y + contentArea.h - currentY)) // Don't exceed content area
        };

        // Only render if child is within visible area
        if (currentY + childSize.h > canvasBox.y && currentY < canvasBox.y + canvasBox.h)
        {
            _children[i]->render(r, childCanvas);
        }

        // Move to next position
        currentY += childSize.h + _style.spacing;
    }
}

Size ScrollableVerticalLayout::measure() const
{
    if (_count == 0) return Size{0, 0};
    
    // Calculate content size (excluding margins)
    uint16_t contentHeight = 0;
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i])
            contentHeight += _children[i]->measure().h;
    }
    
    // Add spacing based on mode
    if (_count > 1)
    {
        switch (_style.spacingMode)
        {
            case SpacingMode::Fixed:
                contentHeight += (_count - 1) * _style.spacing;
                break;
            // For even spacing modes, spacing is calculated dynamically in render()
            default:
                break;
        }
    }
    
    uint16_t contentWidth = 0;
    
    // Find maximum width among children
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i])
        {
            Size childSize = _children[i]->measure();
            if (childSize.w > contentWidth)
                contentWidth = childSize.w;
        }
    }
    
    // Add margins to get total size
    uint16_t totalWidth = contentWidth + _style.marginLeft + _style.marginRight;
    uint16_t totalHeight = contentHeight + _style.marginTop + _style.marginBottom;
    
    return Size{static_cast<uint8_t>(totalWidth), static_cast<uint8_t>(totalHeight)};
}

Widget* ScrollableVerticalLayout::child(size_t index) const
{
    if (index < _count)
        return _children[index];
    return nullptr;
}