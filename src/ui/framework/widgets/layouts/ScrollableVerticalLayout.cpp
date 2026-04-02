#include "ScrollableVerticalLayout.hpp"
#include "../core/Selectable.hpp"
#include <cassert>

namespace ui {
namespace widgets {

void ScrollableVerticalLayout::updateScrollOffset(
    const std::vector<ChildInfo>& children,
    uint16_t visibleHeight)
{
    uint16_t currentY = _style.marginTop;

    for (const auto& child : children)
    {
        auto* widget = child.widget;

        if (widget->isSelectable())
        {
            auto* selectable = static_cast<Selectable*>(widget);

            if (selectable->isFocused())
            {
                uint16_t top = currentY;
                uint16_t bottom = currentY + child.size.h;

                if (top < _scrollOffset)
                {
                    _scrollOffset = top;
                }
                else if (bottom > _scrollOffset + visibleHeight)
                {
                    _scrollOffset = bottom - visibleHeight;
                }

                return;
            }
        }

        currentY += child.size.h + _style.spacing;
    }
}

Rect ScrollableVerticalLayout::applyMargins(Rect box) const
{
    auto safeSub = [](int a, int b) {
        return std::max(0, a - b);
    };

    return {
        static_cast<uint16_t>(box.x + _style.marginLeft),
        static_cast<uint16_t>(box.y + _style.marginTop),
        static_cast<uint16_t>(safeSub(box.w, _style.marginLeft + _style.marginRight)),
        static_cast<uint16_t>(safeSub(box.h, _style.marginTop + _style.marginBottom))
    };
}

std::vector<ScrollableVerticalLayout::ChildInfo>
ScrollableVerticalLayout::collectChildren() const
{
    std::vector<ChildInfo> result;
    result.reserve(getChildCount());

    for (size_t i = 0; i < getChildCount(); i++)
    {
        if (auto* w = getChild(i))
        {
            result.push_back({
                w,
                w->measure()
            });
        }
    }

    return result;
}

uint16_t ScrollableVerticalLayout::computeChildX(uint16_t width, const Rect& area) const
{
    switch (_style.horizontalAlign)
    {
        case HorizontalAlignment::Center:
            return area.x + (area.w - width) / 2;

        case HorizontalAlignment::Right:
            return area.x + area.w - width;

        default:
            return area.x;
    }
}

void ScrollableVerticalLayout::render(Renderer& r, Rect canvasBox)
{
    if (getChildCount() == 0 || canvasBox.w == 0 || canvasBox.h == 0)
        return;

    Rect content = applyMargins(canvasBox);

    auto children = collectChildren();

    updateScrollOffset(children, content.h);

    int currentY = content.y - _scrollOffset;

    for (const auto& child : children)
    {
        auto* widget = child.widget;

        uint16_t width = widget->canExpandHorizontally()
            ? content.w
            : std::min(child.size.w, content.w);

        bool visible =
            currentY + child.size.h > canvasBox.y &&
            currentY < canvasBox.y + canvasBox.h;

        if (visible) {
            uint16_t x = computeChildX(width, content);

            Rect rect = {
                x,
                static_cast<uint16_t>(currentY),
                width,
                child.size.h
            };

            widget->render(r, rect);
        }
        currentY += child.size.h + _style.spacing;
    }
}

Size ScrollableVerticalLayout::measure() const
{
    auto children = collectChildren();

    if (children.empty())
        return {0, 0};

    uint16_t totalHeight = 0;
    uint16_t maxWidth = 0;

    for (const auto& child : children)
    {
        totalHeight += child.size.h;
        maxWidth = std::max(maxWidth, static_cast<uint16_t>(child.size.w));
    }

    if (children.size() > 1)
        totalHeight += (children.size() - 1) * _style.spacing;

    return {
        static_cast<uint16_t>(maxWidth + _style.marginLeft + _style.marginRight),
        static_cast<uint16_t>(totalHeight + _style.marginTop + _style.marginBottom)
    };
}

bool ScrollableVerticalLayout::canExpandHorizontally() const
{
    for (size_t i = 0; i < getChildCount(); i++)
        if (auto* child = getChild(i))
            if (child->canExpandHorizontally())
                return true;

    return false;
}

bool ScrollableVerticalLayout::canExpandVertically() const
{
    return false; // scrolls instead
}

} // namespace widgets
} // namespace ui