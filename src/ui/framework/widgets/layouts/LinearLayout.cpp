#include "LinearLayout.hpp"

namespace ui {
namespace widgets {

Rect LinearLayout::applyMargins(Rect box) const
{
    return {
        uint8_t(box.x + _style.marginLeft),
        uint8_t(box.y + _style.marginTop),
        uint8_t(box.w - _style.marginLeft - _style.marginRight),
        uint8_t(box.h - _style.marginTop - _style.marginBottom)
    };
}

// ---------- Axis helpers ----------

uint16_t LinearLayout::primarySize(Size s) const {
    return (_axis == Axis::Horizontal) ? s.w : s.h;
}

uint16_t LinearLayout::secondarySize(Size s) const {
    return (_axis == Axis::Horizontal) ? s.h : s.w;
}

uint16_t LinearLayout::availablePrimary(Rect r) const {
    return (_axis == Axis::Horizontal) ? r.w : r.h;
}

uint16_t LinearLayout::availableSecondary(Rect r) const {
    return (_axis == Axis::Horizontal) ? r.h : r.w;
}

// ---------- Spacing ----------

LinearLayout::Spacing
LinearLayout::computeSpacing(uint16_t available, uint16_t total, size_t count) const
{
    if (count <= 1)
        return {double(_style.spacing), 0};

    int remaining = std::max(0, int(available) - int(total));

    switch (_style.spacingMode)
    {
        case SpacingMode::Even:
            return {double(remaining) / (count + 1),
                    double(remaining) / (count + 1)};

        case SpacingMode::SpaceBetween:
            return {double(remaining) / (count - 1), 0};

        case SpacingMode::SpaceAround:
            return {double(remaining) / count,
                    double(remaining) / (2 * count)};

        default:
            return {double(_style.spacing), 0};
    }
}

// ---------- Expansion ----------

void LinearLayout::distributeExpansion(std::vector<Child>& children,
                                       uint16_t available) const
{
    uint16_t fixed = 0;
    int expandable = 0;

    for (auto& c : children) {
        if (c.expand) expandable++;
        else fixed += c.minPrimary;
    }

    if (expandable == 0) return;

    int remaining = std::max(0, int(available) - int(fixed) - int(_style.spacing) * int(children.size() - 1));

    int target = remaining / expandable;

    bool changed = true;

    while (changed && expandable > 0)
    {
        changed = false;

        for (auto& c : children)
        {
            if (!c.expand || c.locked) continue;

            if (c.minPrimary > target)
            {
                c.finalPrimary = c.minPrimary;
                c.locked = true;

                remaining -= c.finalPrimary;
                expandable--;

                changed = true;
            }
        }

        if (expandable > 0)
            target = remaining / expandable;
    }

    for (auto& c : children)
        if (c.expand && !c.locked)
            c.finalPrimary = target;
}

// ---------- Layout ----------

std::vector<LinearLayout::LayoutItem>
LinearLayout::computeLayout(Rect content) const
{
    std::vector<LayoutItem> result;

    const size_t count = EnChildCount();
    if (count == 0) return result;

    std::vector<Child> children;
    children.reserve(count);

    // 1. Collect children
    for (size_t i = 0; i < count; i++)
    {
        if (auto* w = EnChild(i))
        {
            Size s = w->measure();

            bool expandPrimary = (_axis == Axis::Horizontal)
                ? w->canExpandHorizontally()
                : w->canExpandVertically();

            bool expandSecondary = (_axis == Axis::Horizontal)
                ? w->canExpandVertically()
                : w->canExpandHorizontally();

            uint16_t p = primarySize(s);
            uint16_t s2 = expandSecondary ? availableSecondary(content) : secondarySize(s);

            children.push_back({w, p, p, s2, expandPrimary});
        }
    }

    // 2. Compute total size
    uint16_t total = 0;
    for (auto& c : children) total += c.finalPrimary;

    uint16_t available = availablePrimary(content);

    // 3. Expansion OR spacing
    bool hasExpand = std::any_of(children.begin(), children.end(),
                                 [](auto& c){ return c.expand; });

    Spacing spacing;

    if (hasExpand)
    {
        distributeExpansion(children, available);
        spacing = {double(_style.spacing), 0};
    }
    else
    {
        spacing = computeSpacing(available, total, children.size());
    }

    // 4. Positioning
    int start = (_axis == Axis::Horizontal) ? content.x : content.y;
    int end   = (_axis == Axis::Horizontal)
            ? (content.x + content.w)
            : (content.y + content.h);

    int pos = start + spacing.leading;

    double error = 0;

    for (size_t i = 0; i < children.size(); i++)
    {
        auto& c = children[i];

        int secPos = (_axis == Axis::Horizontal) ? content.y : content.x;

        // alignment (unchanged)
        if (_axis == Axis::Horizontal)
        {
            if (_style.verticalAlign == VerticalAlignment::Middle)
                secPos += (content.h - c.secondary) / 2;
            else if (_style.verticalAlign == VerticalAlignment::Bottom)
                secPos += content.h - c.secondary;
        }
        else
        {
            if (_style.horizontalAlign == HorizontalAlignment::Center)
                secPos += (content.w - c.secondary) / 2;
            else if (_style.horizontalAlign == HorizontalAlignment::Right)
                secPos += content.w - c.secondary;
        }

        int childStart = pos;
        int childEnd   = pos + c.finalPrimary;

        // SKIP completely off-screen children
        if (childEnd <= start)
        {
            pos += c.finalPrimary;
            continue;
        }

        if (childStart >= end)
        {
            break; // nothing else visible
        }

        // CLAMP visible part
        int visibleStart = std::max(childStart, start);
        int visibleEnd   = std::min(childEnd, end);

        int visibleSize = visibleEnd - visibleStart;

        if (visibleSize <= 0)
        {
            pos += c.finalPrimary;
            continue;
        }

        Rect rect;

        if (_axis == Axis::Horizontal)
        {
            rect = {
                uint8_t(visibleStart),
                uint8_t(secPos),
                uint8_t(visibleSize),
                uint8_t(c.secondary)
            };
        }
        else
        {
            rect = {
                uint8_t(secPos),
                uint8_t(visibleStart),
                uint8_t(c.secondary),
                uint8_t(visibleSize)
            };
        }

        result.push_back({c.widget, rect});

        pos += c.finalPrimary;

        if (i + 1 < children.size())
        {
            int s = std::round(spacing.between);
            pos += s;

            error += spacing.between - s;

            if (error >= 0.5) { pos++; error -= 1.0; }
            else if (error <= -0.5) { pos--; error += 1.0; }
        }
    }

    return result;
}

// ---------- Public API ----------

void LinearLayout::render(Renderer& r, Rect canvasBox)
{
    Rect content = applyMargins(canvasBox);
    auto layout = computeLayout(content);

    for (auto& item : layout)
        if (item.widget)
            item.widget->render(r, item.rect);
}

Size LinearLayout::measure() const
{
    uint16_t primary = 0;
    uint16_t secondary = 0;

    for (size_t i = 0; i < EnChildCount(); i++)
    {
        if (auto* w = EnChild(i))
        {
            Size s = w->measure();
            primary += primarySize(s);
            secondary = std::max(secondary, secondarySize(s));
        }
    }

    if (EnChildCount() > 1 && _style.spacingMode == SpacingMode::Fixed)
        primary += (EnChildCount() - 1) * _style.spacing;

    uint16_t w = (_axis == Axis::Horizontal) ? primary : secondary;
    uint16_t h = (_axis == Axis::Horizontal) ? secondary : primary;

    w += _style.marginLeft + _style.marginRight;
    h += _style.marginTop + _style.marginBottom;

    return {uint8_t(w), uint8_t(h)};
}

bool LinearLayout::canExpandHorizontally() const
{
    if (_axis == Axis::Horizontal && _style.spacingMode != SpacingMode::Fixed)
        return true;

    for (size_t i = 0; i < EnChildCount(); i++)
        if (auto* w = EnChild(i))
            if (w->canExpandHorizontally())
                return true;

    return false;
}

bool LinearLayout::canExpandVertically() const
{
    if (_axis == Axis::Vertical && _style.spacingMode != SpacingMode::Fixed)
        return true;

    for (size_t i = 0; i < EnChildCount(); i++)
        if (auto* w = EnChild(i))
            if (w->canExpandVertically())
                return true;

    return false;
}

} // namespace widgets
} // namespace ui