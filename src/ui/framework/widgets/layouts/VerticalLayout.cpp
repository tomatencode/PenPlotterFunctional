#include "VerticalLayout.hpp"

namespace ui {
namespace widgets {

VerticalLayout::VerticalLayout(const VerticalLayoutStyle& style, std::vector<std::unique_ptr<Widget>>&& children)
    : LayoutWidget(std::move(children)), _style(style)
{
}

Size VerticalLayout::measure() const
{
    const size_t count = childCount();
    if (count == 0) return Size{0, 0};
    
    // Calculate content size (excluding margins)
    uint16_t contentHeight = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = child(i))
            contentHeight += w->measure().h;
    }
    
    // Add spacing based on mode
    if (count > 1)
    {
        switch (_style.spacingMode)
        {
            case SpacingMode::Fixed:
                contentHeight += (count - 1) * _style.spacing;
                break;
            // For even spacing modes, spacing is calculated dynamically in render()
            default:
                break;
        }
    }
    
    uint16_t contentWidth = 0;
    
    // Find maximum width among children
    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = child(i))
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

bool VerticalLayout::canExpandHorizontally() const {
    // VerticalLayout can expand horizontally if any child can expand horizontally
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

bool VerticalLayout::canExpandVertically() const {
    // VerticalLayout can expand vertically if any child can expand vertically or if spacing mode allows expansion
    if (_style.spacingMode != SpacingMode::Fixed)
        return true; // Spacing can expand to fill available space

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

double VerticalLayout::getSpacing(uint16_t availableHeight) const
{
    const size_t count = childCount();
    if (count < 1) return 0;
    
    uint16_t totalChildHeight = 0;
    for (size_t i = 0; i < count; i++)
    {
        if (Widget* w = child(i))
            totalChildHeight += w->measure().h;
    }
    
    double remainingSpace = availableHeight - totalChildHeight;
    
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

void VerticalLayout::render(Renderer& r, Rect canvasBox)
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
    double idealSpacing = getSpacing(contentArea.h);
    double spacingError = 0.0; // For accumulating fractional spacing

    // Calculate starting Y position
    int currentY = contentArea.y;
    
    // add initial spacing for even distribution modes
    if (_style.spacingMode == SpacingMode::Even) {
        currentY += std::round(idealSpacing);
        spacingError += idealSpacing - std::round(idealSpacing);
    } 
    else if (_style.spacingMode == SpacingMode::SpaceAround) {
        currentY += std::round(idealSpacing / 2);
        spacingError += (idealSpacing / 2) - std::round(idealSpacing / 2);
    }

    // Render each child
    for (size_t i = 0; i < count; i++)
    {
        Widget* childWidget = child(i);
        if (childWidget == nullptr)
            continue;

        Size minChildSize = childWidget->measure();
        bool canExpandHorizontally = childWidget->canExpandHorizontally();

        Size desiredChildSize = minChildSize;
        if (canExpandHorizontally)
            desiredChildSize.w = contentArea.w; // Fill available width

        Size childSize = {std::min(desiredChildSize.w, static_cast<uint8_t>(contentArea.w)),
                          std::min(desiredChildSize.h, static_cast<uint8_t>(contentArea.h - currentY))};

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
        if (currentY < canvasBox.y + canvasBox.h)
        {
            childWidget->render(r, childCanvas);
        }

        // Move to next position
        currentY += childSize.h + std::round(idealSpacing);
        spacingError += idealSpacing - std::round(idealSpacing);
        // If accumulated error exceeds 0.5 pixel, add extra spacing
        if (spacingError >= 0.5) {
            currentY += 1;
            spacingError -= 1.0;
        } 
        else if (spacingError <= -0.5) {
            currentY -= 1;
            spacingError += 1.0;
        }
    }
}

} // namespace widgets
} // namespace ui