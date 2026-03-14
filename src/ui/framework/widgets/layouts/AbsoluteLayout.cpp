#include "AbsoluteLayout.hpp"
#include <algorithm>

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
        int childAbsX = layoutDrawRect.x + _children[i].box.x;
        int childAbsY = layoutDrawRect.y + _children[i].box.y;

        int startX = childAbsX;
        int startY = childAbsY;

        switch (_children[i].align.horizontal)
        {
            case HorizontalAlignment::Center:
                startX += (_children[i].box.w - childSize.w) / 2;
                break;

            case HorizontalAlignment::Right:
                startX += _children[i].box.w - childSize.w;
                break;

            default:
                break;
        }

        switch (_children[i].align.vertical)
        {
            case VerticalAlignment::Middle:
                startY += (_children[i].box.h - childSize.h) / 2;
                break;

            case VerticalAlignment::Bottom:
                startY += _children[i].box.h - childSize.h;
                break;

            default:
                break;
        }

        // clipping against parent canvas
        int remainingW = std::max(0, int(layoutDrawRect.w) - int(_children[i].box.x));
        int remainingH = std::max(0, int(layoutDrawRect.h) - int(_children[i].box.y));

        int w = std::min<int>(childSize.w, remainingW);
        int h = std::min<int>(childSize.h, remainingH);

        Rect drawRect{
            static_cast<uint8_t>(startX),
            static_cast<uint8_t>(startY),
            static_cast<uint8_t>(w),
            static_cast<uint8_t>(h)
        };

        _children[i].widget->render(r, drawRect);
    }
}
