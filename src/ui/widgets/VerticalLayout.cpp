#include "VerticalLayout.hpp"

VerticalLayout::VerticalLayout(Widget* children[], size_t count)
    : _count(0)
{
    for (size_t i = 0; i < count && i < MAX_LAYOUT_CHILDREN; i++)
    {
        if (children[i] != nullptr)
            _children[_count++] = children[i];
    }
}

Size VerticalLayout::measure() const
{
    // Width fills available space, height is sum of children
    // Parent determines actual width via canvasBox during render
    return Size{0, static_cast<uint8_t>(getTotalHeight())};
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
    return totalHeight;
}

void VerticalLayout::render(Renderer& r, Rect canvasBox)
{
    if (canvasBox.w == 0 || canvasBox.h == 0)
        return; // nothing visible

    int currentY = canvasBox.y;

    // Render each child stacked vertically
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i] == nullptr)
            continue;

        Size childSize = _children[i]->measure();

        // Create a canvas for this child
        Rect childCanvas = {
            canvasBox.x,
            static_cast<uint8_t>(currentY),
            canvasBox.w,
            childSize.h
        };

        // Only render if child is within visible area
        if (currentY < canvasBox.y + canvasBox.h)
        {
            _children[i]->render(r, childCanvas);
        }

        currentY += childSize.h;
    }
}
