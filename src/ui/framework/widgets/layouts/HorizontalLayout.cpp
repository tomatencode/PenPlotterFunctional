#include "HorizontalLayout.hpp"

namespace ui {
namespace widgets {

HorizontalLayout::HorizontalLayout(const HorizontalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children)
    : LayoutWidget(std::move(children)), _style(style)
{
}

Size HorizontalLayout::measure() const
{
    const size_t count = childCount();
    if (count == 0) return Size{0, 0};
    
    // Calculate content size (excluding margins)
    uint16_t contentWidth = 0;

    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = child(i))
            contentWidth += w->measure().w;
    }
    
    // Add spacing based on mode
    if (count > 1)
    {
        switch (_style.spacingMode)
        {
            case SpacingMode::Fixed:
                contentWidth += (count - 1) * _style.spacing;
                break;
            // For even spacing modes, spacing is calculated dynamically in render()
            default:
                break;
        }
    }
    
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
    uint16_t totalWidth = contentWidth + _style.marginLeft + _style.marginRight;
    uint16_t totalHeight = contentHeight + _style.marginTop + _style.marginBottom;
    
    return Size{static_cast<uint8_t>(contentWidth), static_cast<uint8_t>(totalHeight)};
}

bool HorizontalLayout::canExpandHorizontally() const {
    // HorizontalLayout can expand horizontally if any child can expand horizontally or if spacing mode allows expansion
    if (_style.spacingMode != SpacingMode::Fixed)
        return true; // Spacing modes other than Fixed allow expansion to fill available space
    
    for (size_t i = 0; i < childCount(); i++)
    {
        if (Widget* w = child(i))
        {
            if (w->canExpandHorizontally())
                return true;
        }
    }
    return false;
}

bool HorizontalLayout::canExpandVertically() const {
    // HorizontalLayout can expand vertically if any child can expand vertically
    for (size_t i = 0; i < childCount(); i++)
    {
        if (Widget* w = child(i))
        {
            if (w->canExpandVertically())
                return true;
        }
    }
    return false;
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
    
    switch (_style.spacingMode)
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
            return _style.spacing;
    }
}

void HorizontalLayout::render(Renderer& r, Rect canvasBox)
{
    const size_t count = childCount();
    if (count == 0 || canvasBox.w == 0 || canvasBox.h == 0)
        return; // nothing to render

    // Apply margins to create content area
    Rect contentArea = {
        static_cast<uint8_t>(canvasBox.x + _style.marginLeft),
        static_cast<uint8_t>(canvasBox.y + _style.marginTop),
        static_cast<uint8_t>(canvasBox.w - _style.marginLeft - _style.marginRight),
        static_cast<uint8_t>(canvasBox.h - _style.marginTop - _style.marginBottom)
    };

    // Calculate dynamic spacing if needed
    double idealSpacing = getSpacing(contentArea.w);
    double spacingError = 0.0; // For accumulating fractional spacing
    
    // Calculate starting X position
    int currentX = contentArea.x;
    
    // add initial spacing for even distribution modes
    if (_style.spacingMode == SpacingMode::Even) {
        currentX += std::round(idealSpacing);
        spacingError += idealSpacing - std::round(idealSpacing);
    }
    else if (_style.spacingMode == SpacingMode::SpaceAround) {
        currentX += std::round(idealSpacing / 2);
        spacingError += (idealSpacing / 2) - std::round(idealSpacing / 2);
    }

    // Render each child
    for (size_t i = 0; i < count; i++)
    {
        Widget* childWidget = child(i);
        if (childWidget == nullptr)
            continue;

        Size minChildSize = childWidget->measure();
        bool canExpandVertically = childWidget->canExpandVertically();

        Size desiredChildSize = minChildSize;
        if (canExpandVertically)
            desiredChildSize.h = contentArea.h; // Fill available height

        Size childSize = {std::min(desiredChildSize.w, static_cast<uint8_t>(contentArea.w - currentX)),
                          std::min(desiredChildSize.h, static_cast<uint8_t>(contentArea.h))};

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
            childSize.h
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