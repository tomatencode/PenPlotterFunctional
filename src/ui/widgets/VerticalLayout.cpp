#include "VerticalLayout.hpp"
#include "../widgetSystem/WidgetUtils.hpp"

VerticalLayout::VerticalLayout(Rect box, Alignment align, Widget* children[], size_t count)
    : Widget(box, align), _count(count)
{
    for (size_t i = 0; i < _count && i < MAX_LAYOUT_CHILDREN; i++)
        _children[i] = children[i];
}

Size VerticalLayout::measure() const
{
    return Size{box().w, box().h};
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
    // Compute final drawing rectangle including alignment + clipping
    Rect drawRect = computeContentAlignment(box(), align(), measure(), canvasBox);

    if (drawRect.w == 0 || drawRect.h == 0)
        return; // nothing visible

    int currentY = drawRect.y;

    // Render each child stacked vertically
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i] == nullptr)
            continue;

        Size childSize = _children[i]->measure();

        // Create a canvas for this child
        Rect childCanvas = {
            drawRect.x,
            static_cast<uint8_t>(currentY),
            drawRect.w,
            childSize.h
        };

        // Only render if child is within visible area
        if (currentY < drawRect.y + drawRect.h)
        {
            _children[i]->render(r, childCanvas);
        }

        currentY += childSize.h;
    }
}
