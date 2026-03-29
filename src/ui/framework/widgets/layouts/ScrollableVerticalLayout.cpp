#include "ScrollableVerticalLayout.hpp"
#include "../core/Selectable.hpp"
#include <cassert>

namespace ui {
namespace widgets {

void ScrollableVerticalLayout::updateScrollOffset(
    const std::vector<ChildInfo>& children,
    uint16_t visibleHeight)
{
    int currentY = _style.marginTop;

    for (const auto& child : children)
    {
        auto* widget = child.widget;

        if (widget->isSelectable())
        {
            auto* selectable = static_cast<Selectable*>(widget);

            if (selectable->isFocused())
            {
                Serial.println("Focused child at offset " + String(currentY) + " with offset " + String(_scrollOffset));

                int top = currentY;
                int bottom = currentY + child.size.h;

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
    return {
        static_cast<uint8_t>(box.x + _style.marginLeft),
        static_cast<uint8_t>(box.y + _style.marginTop),
        static_cast<uint8_t>(box.w - _style.marginLeft - _style.marginRight),
        static_cast<uint8_t>(box.h - _style.marginTop - _style.marginBottom)
    };
}

std::vector<ScrollableVerticalLayout::ChildInfo>
ScrollableVerticalLayout::collectChildren() const
{
    std::vector<ChildInfo> result;
    result.reserve(EnChildCount());

    for (size_t i = 0; i < EnChildCount(); i++)
    {
        if (auto* w = EnChild(i))
        {
            result.push_back({
                w,
                w->measure()
            });
        }
    }

    return result;
}

int ScrollableVerticalLayout::computeChildX(uint16_t width, const Rect& area) const
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
    if (EnChildCount() == 0 || canvasBox.w == 0 || canvasBox.h == 0)
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

        int x = computeChildX(width, content);

        Rect rect = {
            static_cast<uint8_t>(x),
            static_cast<uint8_t>(currentY),
            static_cast<uint8_t>(width),
            child.size.h
        };

        // visibility check (cleaner)
        bool visible =
            currentY + child.size.h > canvasBox.y &&
            currentY < canvasBox.y + canvasBox.h;

        if (visible)
            widget->render(r, rect);

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
        static_cast<uint8_t>(maxWidth + _style.marginLeft + _style.marginRight),
        static_cast<uint8_t>(totalHeight + _style.marginTop + _style.marginBottom)
    };
}

bool ScrollableVerticalLayout::canExpandHorizontally() const
{
    for (size_t i = 0; i < EnChildCount(); i++)
        if (auto* w = EnChild(i))
            if (w->canExpandHorizontally())
                return true;

    return false;
}

bool ScrollableVerticalLayout::canExpandVertically() const
{
    return false; // scrolls instead
}

} // namespace widgets
} // namespace ui