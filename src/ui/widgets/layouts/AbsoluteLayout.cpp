#include "AbsoluteLayout.hpp"
#include "../core/WidgetUtils.hpp"

AbsoluteLayout::AbsoluteLayout(Rect layoutBox)
    : _layoutBox(layoutBox), _count(0)
{
}

void AbsoluteLayout::addChild(Widget* widget, Rect box, Alignment align)
{
    if (_count < MAX_CHILDREN && widget)
    {
        _children[_count] = {widget, box, align};
        _count++;
    }
}

Size AbsoluteLayout::measure() const
{
    return Size{_layoutBox.w, _layoutBox.h};
}

Widget* AbsoluteLayout::child(size_t index) const
{
    if (index < _count)
        return _children[index].widget;
    return nullptr;
}

void AbsoluteLayout::render(Renderer& r, Rect parentCanvas)
{
    // Compute layout's absolute position
    int absX = parentCanvas.x + _layoutBox.x;
    int absY = parentCanvas.y + _layoutBox.y;

    // Create layout's draw rect (clipped to parent)
    Rect layoutDrawRect{
        static_cast<uint8_t>(absX),
        static_cast<uint8_t>(absY),
        _layoutBox.w,
        _layoutBox.h
    };

    // Render each child at its specified position
    for (size_t i = 0; i < _count; i++)
    {
        if (_children[i].widget == nullptr)
            continue;

        Size childSize = _children[i].widget->measure();
        
        // Compute child's position with alignment inside its box
        Rect drawRect = computeContentAlignment(
            _children[i].box,
            _children[i].align,
            childSize,
            layoutDrawRect
        );

        _children[i].widget->render(r, drawRect);
    }
}
