#include "VerticalLayout.hpp"

VerticalLayout::VerticalLayout(Widget* children[], size_t count, const LayoutStyle& style)
    : _count(0), _style(style)
{
    for (size_t i = 0; i < count && i < MAX_LAYOUT_CHILDREN; i++)
    {
        if (children[i] != nullptr)
            _children[_count++] = children[i];
    }
}

Size VerticalLayout::measure() const
{
    if (_count == 0) return Size{0, 0};
    
    // Calculate content size (excluding margins)
    uint16_t contentHeight = getTotalHeight();
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

Widget* VerticalLayout::child(size_t index) const
{
    if (index < _count)
        return _children[index];
    return nullptr;
}

uint16_t VerticalLayout::getTotalHeight() const
{
    uint16_t totalHeight = 0;
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i])
            totalHeight += _children[i]->measure().h;
    }
    
    // Add spacing based on mode
    if (_count > 1)
    {
        switch (_style.spacingMode)
        {
            case SpacingMode::Fixed:
                totalHeight += (_count - 1) * _style.spacing;
                break;
            // For even spacing modes, spacing is calculated dynamically in render()
            default:
                break;
        }
    }
    
    return totalHeight;
}

uint8_t VerticalLayout::getSpacing(uint16_t availableHeight) const
{
    if (_count <= 1) return 0;
    
    uint16_t totalChildHeight = 0;
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i])
            totalChildHeight += _children[i]->measure().h;
    }
    
    uint16_t remainingSpace = availableHeight - totalChildHeight;
    
    switch (_style.spacingMode)
    {
        case SpacingMode::Even:
            // Distribute remaining space evenly between all gaps (including edges)
            return remainingSpace / (_count + 1);
            
        case SpacingMode::SpaceBetween:
            // Space between children only, no space at edges
            return remainingSpace / (_count - 1);
            
        case SpacingMode::SpaceAround:
            // Space around each child, half at edges
            return remainingSpace / _count;
            
        default: // Fixed
            return _style.spacing;
    }
}

void VerticalLayout::render(Renderer& r, Rect canvasBox)
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

    // Calculate dynamic spacing if needed
    uint8_t spacing = getSpacing(contentArea.h);
    
    // Calculate starting Y position
    int currentY = contentArea.y;
    
    // For even spacing with edge space, add initial spacing
    if (_style.spacingMode == SpacingMode::Even || _style.spacingMode == SpacingMode::SpaceAround)
    {
        currentY += spacing;
    }

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
            contentArea.w - static_cast<uint8_t>(childX),  // Full available width
            childSize.h
        };

        // Only render if child is within visible area
        if (currentY < canvasBox.y + canvasBox.h)
        {
            _children[i]->render(r, childCanvas);
        }

        // Move to next position
        currentY += childSize.h + spacing;
        
        // For SpaceAround, add spacing after each child
        if (_style.spacingMode == SpacingMode::SpaceAround && i < _count - 1)
        {
            currentY += spacing;
        }
    }
}
