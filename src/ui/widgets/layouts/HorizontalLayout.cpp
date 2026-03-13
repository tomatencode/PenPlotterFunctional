#include "HorizontalLayout.hpp"

HorizontalLayout::HorizontalLayout(Widget* children[], size_t count, const LayoutStyle& style)
    : _count(0), _style(style)
{
    for (size_t i = 0; i < count && i < MAX_LAYOUT_CHILDREN; i++)
    {
        if (children[i] != nullptr)
            _children[_count++] = children[i];
    }
}

Size HorizontalLayout::measure() const
{
    if (_count == 0) return Size{0, 0};
    
    // Calculate content size (excluding margins)
    uint16_t contentWidth = getTotalWidth();
    uint16_t contentHeight = 0;
    
    // Find maximum height among children
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i])
        {
            Size childSize = _children[i]->measure();
            if (childSize.h > contentHeight)
                contentHeight = childSize.h;
        }
    }
    
    // Add margins to get total size
    uint16_t totalWidth = contentWidth + _style.marginLeft + _style.marginRight;
    uint16_t totalHeight = contentHeight + _style.marginTop + _style.marginBottom;
    
    return Size{static_cast<uint8_t>(totalWidth), static_cast<uint8_t>(totalHeight)};
}

Widget* HorizontalLayout::child(size_t index) const
{
    if (index < _count)
        return _children[index];
    return nullptr;
}

uint16_t HorizontalLayout::getTotalWidth() const
{
    uint16_t totalWidth = 0;
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i])
            totalWidth += _children[i]->measure().w;
    }
    
    // Add spacing based on mode
    if (_count > 1)
    {
        switch (_style.spacingMode)
        {
            case SpacingMode::Fixed:
                totalWidth += (_count - 1) * _style.spacing;
                break;
            // For even spacing modes, spacing is calculated dynamically in render()
            default:
                break;
        }
    }
    
    return totalWidth;
}

uint8_t HorizontalLayout::getSpacing(uint16_t availableWidth) const
{
    if (_count <= 1) return 0;
    
    uint16_t totalChildWidth = 0;
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i])
            totalChildWidth += _children[i]->measure().w;
    }
    
    uint16_t remainingSpace = availableWidth - totalChildWidth;
    
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

void HorizontalLayout::render(Renderer& r, Rect canvasBox)
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
    uint8_t spacing = getSpacing(contentArea.w);
    
    // Calculate starting X position
    int currentX = contentArea.x;
    
    // For even spacing with edge space, add initial spacing
    if (_style.spacingMode == SpacingMode::Even || _style.spacingMode == SpacingMode::SpaceAround)
    {
        currentX += spacing;
    }

    // Render each child
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i] == nullptr)
            continue;

        Size childSize = _children[i]->measure();

        // Calculate Y position based on vertical alignment
        int childY = contentArea.y;
        switch (_style.verticalAlign)
        {
            case VerticalAlignment::Middle:
                childY += (contentArea.h - childSize.h) / 2;
                break;
            case VerticalAlignment::Bottom:
                childY += contentArea.h - childSize.h;
                break;
            default: // Top
                break;
        }

        // Create canvas for this child
        // HorizontalLayout fills vertical space, so give child full available height
        Rect childCanvas = {
            static_cast<uint8_t>(currentX),
            static_cast<uint8_t>(childY),
            childSize.w,
            contentArea.h  // Full available height
        };

        // Only render if child is within visible area
        if (currentX < canvasBox.x + canvasBox.w)
        {
            _children[i]->render(r, childCanvas);
        }

        // Move to next position
        currentX += childSize.w + spacing;
        
        // For SpaceAround, add spacing after each child
        if (_style.spacingMode == SpacingMode::SpaceAround && i < _count - 1)
        {
            currentX += spacing;
        }
    }
}