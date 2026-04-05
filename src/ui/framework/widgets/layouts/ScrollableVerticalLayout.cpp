#include "ScrollableVerticalLayout.hpp"
#include "../core/ISelectable.hpp"

namespace ui {
namespace widgets {

// ---------- Helpers ----------

Box ScrollableVerticalLayout::applyMargins(Box box) const
{
    auto safeSub = [](int a, int b) { return std::max(0, a - b); };
    return {
        static_cast<int16_t>(box.x + _style.marginLeft),
        static_cast<int16_t>(box.y + _style.marginTop),
        static_cast<uint16_t>(safeSub(box.w, _style.marginLeft + _style.marginRight)),
        static_cast<uint16_t>(safeSub(box.h, _style.marginTop + _style.marginBottom))
    };
}

bool ScrollableVerticalLayout::containsFocusedWidget(Widget* widget) const
{
    if (!widget) return false;

    if (auto* selectable = widget->tryGetSelectable())
        if (selectable->isFocused())
            return true;

    for (size_t i = 0; i < widget->getChildCount(); i++)
        if (containsFocusedWidget(widget->getChild(i)))
            return true;

    return false;
}

// ---------- Layout ----------

std::vector<ScrollableVerticalLayout::ChildInfo>
ScrollableVerticalLayout::collectChildren(Box content) const
{
    std::vector<ChildInfo> result;
    result.reserve(getChildCount());

    for (size_t i = 0; i < getChildCount(); i++)
    {
        if (auto* w = getChild(i))
        {
            Size s = w->measure();
            uint16_t width = w->canExpandHorizontally()
                ? content.w
                : std::min(s.w, content.w);
            result.push_back({w, width, s.h});
        }
    }

    return result;
}

std::vector<ScrollableVerticalLayout::LayoutItem>
ScrollableVerticalLayout::computeLayout(Box content)
{
    auto children = collectChildren(content);

    // 1. Update scroll offset so the focused child stays visible.
    //    currentY is content-relative (0 = top of content area).
    uint16_t currentY = 0;
    for (const auto& child : children)
    {
        if (containsFocusedWidget(child.widget))
        {
            if (currentY < _scrollOffset)
                _scrollOffset = currentY;
            else if (currentY + child.height > _scrollOffset + content.h)
                _scrollOffset = currentY + child.height - content.h;
            break;
        }
        currentY += child.height + _style.spacing;
    }

    // 2. Build layout items, converting content-relative Y to screen Y.
    std::vector<LayoutItem> result;
    result.reserve(children.size());

    currentY = 0;
    for (const auto& child : children)
    {
        int screenY = static_cast<int>(content.y)
                    + static_cast<int>(currentY)
                    - static_cast<int>(_scrollOffset);

        int contentBottom = static_cast<int>(content.y) + static_cast<int>(content.h);
        bool visible = screenY + static_cast<int>(child.height) > static_cast<int>(content.y)
                    && screenY < contentBottom;

        if (visible)
        {
            int16_t x;
            switch (_style.horizontalAlign)
            {
                case HorizontalAlignment::Center:
                    x = content.x + (content.w - child.width) / 2;
                    break;
                case HorizontalAlignment::Right:
                    x = content.x + content.w - child.width;
                    break;
                default:
                    x = content.x;
                    break;
            }

            result.push_back({child.widget, Box{
                x,
                static_cast<int16_t>(screenY),
                child.width,
                child.height
            }});
        }

        currentY += child.height + _style.spacing;
    }

    return result;
}

// ---------- Public API ----------

void ScrollableVerticalLayout::render(Renderer& r, Box canvasBox)
{
    if (getChildCount() == 0 || canvasBox.w == 0 || canvasBox.h == 0)
        return;

    Box content = applyMargins(canvasBox);
    auto layout = computeLayout(content);

    for (auto& item : layout)
        if (item.widget)
            item.widget->render(r, item.rect);
}

Size ScrollableVerticalLayout::measure() const
{
    uint16_t totalHeight = 0;
    uint16_t maxWidth = 0;

    for (size_t i = 0; i < getChildCount(); i++)
    {
        if (auto* w = getChild(i))
        {
            Size s = w->measure();
            totalHeight += s.h;
            maxWidth = std::max(maxWidth, s.w);
        }
    }

    if (getChildCount() > 1)
        totalHeight += static_cast<uint16_t>((getChildCount() - 1) * _style.spacing);

    return {
        static_cast<uint16_t>(maxWidth + _style.marginLeft + _style.marginRight),
        static_cast<uint16_t>(totalHeight + _style.marginTop + _style.marginBottom)
    };
}

bool ScrollableVerticalLayout::canExpandHorizontally() const
{
    for (size_t i = 0; i < getChildCount(); i++)
        if (auto* w = getChild(i))
            if (w->canExpandHorizontally())
                return true;
    return false;
}

bool ScrollableVerticalLayout::canExpandVertically() const
{
    return false; // scrolls instead of expanding
}

} // namespace widgets
} // namespace ui