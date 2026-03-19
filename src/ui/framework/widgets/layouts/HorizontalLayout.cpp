#include "HorizontalLayout.hpp"

namespace ui {
namespace widgets {

HorizontalLayout::HorizontalLayout(std::vector<std::unique_ptr<Widget>>&& children, const LayoutStyle& style)
    : LayoutWidget(std::move(children), style)
{
}

Size HorizontalLayout::measure() const
{
    const size_t count = childCount();
    if (count == 0) return Size{0, 0};
    
    // Calculate content size (excluding margins)
    uint16_t contentWidth = getTotalWidth();
    uint16_t contentHeight = 0;
    
    // Find maximum height among children
    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = child(i))
        {
            Size childSize = w->measure();
            if (childSize.h > contentHeight)
                contentHeight = childSize.h;
        }
    }
    
    // Add margins to get total size
    uint16_t totalWidth = contentWidth + style().marginLeft + style().marginRight;
    uint16_t totalHeight = contentHeight + style().marginTop + style().marginBottom;
    
    return Size{static_cast<uint8_t>(totalWidth), static_cast<uint8_t>(totalHeight)};
}

uint16_t HorizontalLayout::getTotalWidth() const
{
    uint16_t totalWidth = 0;
    const size_t count = childCount();

    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = child(i))
            totalWidth += w->measure().w;
    }
    
    // Add spacing based on mode
    if (count > 1)
    {
        switch (style().spacingMode)
        {
            case SpacingMode::Fixed:
                totalWidth += (count - 1) * style().spacing;
                break;
            // For even spacing modes, spacing is calculated dynamically in render()
            default:
                break;
        }
    }
    
    return totalWidth;
}

double HorizontalLayout::getSpacing(uint16_t availableWidth) const
{
    const size_t count = childCount();
    if (count <= 1) return 0;
    
    uint16_t totalChildWidth = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = child(i))
            totalChildWidth += w->measure().w;
    }
    
    double remainingSpace = availableWidth - totalChildWidth;
    
    switch (style().spacingMode)
    {
        case SpacingMode::Even:
            // Distribute remaining space evenly between all gaps (including edges)
            return remainingSpace / (count + 1);
            
        case SpacingMode::SpaceBetween:
            // Space between children only, no space at edges
            return remainingSpace / (count - 1);
            
        case SpacingMode::SpaceAround:
            // Space around each child, half at edges
            return remainingSpace / count;
            
        default: // Fixed
            return style().spacing;
    }
}

void HorizontalLayout::render(Renderer& r, Rect canvasBox)
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

    // Calculate dynamic spacing if needed
    double idealSpacing = getSpacing(contentArea.w);
    double spacingError = 0.0; // For accumulating fractional spacing
    
    // Calculate starting X position
    int currentX = contentArea.x;
    
    // add initial spacing for even distribution modes
    if (style().spacingMode == SpacingMode::Even) {
        currentX += std::round(idealSpacing);
        spacingError += idealSpacing - std::round(idealSpacing);
    }
    else if (style().spacingMode == SpacingMode::SpaceAround) {
        currentX += std::round(idealSpacing / 2);
        spacingError += (idealSpacing / 2) - std::round(idealSpacing / 2);
    }

    // Render each child
    for (size_t i = 0; i < count; i++)
    {
        Widget* childWidget = child(i);
        if (childWidget == nullptr)
            continue;

        Size childSize = childWidget->measure();

        // Calculate Y position based on vertical alignment
        int childY = contentArea.y;
        switch (style().verticalAlign)
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
            childWidget->render(r, childCanvas);
        }

        // Move to next position
        currentX += childSize.w + std::round(idealSpacing);
        spacingError += idealSpacing - std::round(idealSpacing);
        // If accumulated error exceeds 0.5 pixel, add extra spacing
        if (spacingError >= 0.5)
        {
            currentX += 1;
            spacingError -= 1.0;
        } else if (spacingError <= -0.5)
        {
            currentX -= 1;
            spacingError += 1.0;
        }
    }
}

} // namespace widgets
} // namespace ui