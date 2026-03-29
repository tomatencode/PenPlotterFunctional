#include "HorizontalLayout.hpp"

namespace ui {
namespace widgets {

Rect HorizontalLayout::applyMargins(Rect box) const
{
    return {
        static_cast<uint8_t>(box.x + _style.marginLeft),
        static_cast<uint8_t>(box.y + _style.marginTop),
        static_cast<uint8_t>(box.w - _style.marginLeft - _style.marginRight),
        static_cast<uint8_t>(box.h - _style.marginTop - _style.marginBottom)
    };
}

HorizontalLayout::SpacingInfo HorizontalLayout::computeSpacing(uint16_t availableWidth,
                                        uint16_t totalChildWidth,
                                        size_t count) const
{
    if (count <= 1)
        return {static_cast<double>(_style.spacing), 0};

    int remaining = static_cast<int>(availableWidth) - static_cast<int>(totalChildWidth);
    remaining = std::max(0, remaining);

    switch (_style.spacingMode)
    {
        case SpacingMode::Even:
            return {static_cast<double>(remaining) / (count + 1),
                    static_cast<double>(remaining) / (count + 1)};

        case SpacingMode::SpaceBetween:
            return {static_cast<double>(remaining) / (count - 1), 0};

        case SpacingMode::SpaceAround:
            return {static_cast<double>(remaining) / count,
                    static_cast<double>(remaining) / (2 * count)};
        default:
            return {static_cast<double>(_style.spacing), 0};
    }
}

std::vector<HorizontalLayout::ChildInfo>
HorizontalLayout::expandExpandableChildren(std::vector<ChildInfo> children,
                                           uint16_t availableWidth) const {

    uint16_t totalSpacing = 0;
    totalSpacing = _style.spacing * (children.size() - 1);

    uint16_t fixedWidth = 0;
    for (auto& c : children)
        if (!c.expand)
            fixedWidth += c.minWidth;

    int expandebleCount = 0;
    for (auto& c : children)
        if (c.expand)
            expandebleCount++;

    int remainingWidth = availableWidth - fixedWidth - totalSpacing;

    remainingWidth = std::max(0, remainingWidth);

    bool changed = true;
    uint16_t target = remainingWidth / expandebleCount;
    while (changed)
    {
        changed = false;
        for (auto& c : children)
        {
            if (!c.expand || c.locked)
                continue;

            if (c.minWidth > target)
            {
                c.width = c.minWidth;
                c.locked = true;

                remainingWidth -= c.width;
                expandebleCount--;

                changed = true;
            }
        }

        target = remainingWidth / expandebleCount;
    }

    for (auto& c : children)
    {
        if (c.expand && !c.locked)
            c.width = target;
    }

    return children;                              
}
                                                    
std::vector<HorizontalLayout::LayoutItem> HorizontalLayout::computeLayout(Rect contentArea) const
{
    std::vector<LayoutItem> result;

    const size_t count = EnChildCount();
    if (count == 0)
        return result;

    std::vector<ChildInfo> children;
    children.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        if (auto* w = EnChild(i))
        {
            Size s = w->measure();

            children.push_back({
                w,
                s.w,
                s.w,
                static_cast<uint16_t>(w->canExpandVertically() ? contentArea.h : s.h),
                w->canExpandHorizontally(),
                false
            });
        }
    }


    SpacingInfo spacing = {0, 0};
    if (!std::any_of(children.begin(), children.end(), [](const ChildInfo& c){ return c.expand; }))
    {
        uint16_t totalChildWidth = 0;
        for (const auto& c : children)
            totalChildWidth += c.width;

        spacing = computeSpacing(contentArea.w, totalChildWidth, children.size());
    }
    else
    {
        spacing = {
            static_cast<double>(_style.spacing),
            0.0
        };

        children = expandExpandableChildren(children, contentArea.w);
    }

    int x = contentArea.x + spacing.around;

    double spacingAcumulatedError = 0.0;

    for (size_t i = 0; i < children.size(); i++)
    {
        auto& c = children[i];

        int y = contentArea.y;

        switch (_style.verticalAlign)
        {
            case VerticalAlignment::Middle:
                y += (contentArea.h - c.height) / 2;
                break;

            case VerticalAlignment::Bottom:
                y += contentArea.h - c.height;
                break;

            default:
                break;
        }

        result.push_back({
            c.widget,
            Rect{
                static_cast<uint8_t>(x),
                static_cast<uint8_t>(y),
                static_cast<uint8_t>(c.width),
                static_cast<uint8_t>(c.height)
            }
        });

        x += c.width;

        if (i + 1 < children.size())
            x += std::round(spacing.between);
            spacingAcumulatedError += spacing.between - std::round(spacing.between);
        
        if (spacingAcumulatedError >= 0.5) {
            x += 1;
            spacingAcumulatedError -= 0.5;
        }
        else if (spacingAcumulatedError <= -0.5) {
            x -= 1;
            spacingAcumulatedError += 0.5;
        }
    }

    return result;
}

void HorizontalLayout::render(Renderer& r, Rect canvasBox)
{
    if (EnChildCount() == 0)
        return;

    Rect content = applyMargins(canvasBox);

    auto layout = computeLayout(content);

    for (auto& item : layout)
    {
        if (item.widget)
            item.widget->render(r, item.rect);
    }
}

Size HorizontalLayout::measure() const
{
    uint16_t totalWidth = 0;
    uint16_t maxHeight = 0;

    const size_t count = EnChildCount();

    for (size_t i = 0; i < count; i++)
    {
        if (auto* w = EnChild(i))
        {
            Size s = w->measure();

            totalWidth += s.w;
            maxHeight = std::max(maxHeight, static_cast<uint16_t>(s.h));
        }
    }

    if (count > 1 && _style.spacingMode == SpacingMode::Fixed)
        totalWidth += (count - 1) * _style.spacing;

    totalWidth += _style.marginLeft + _style.marginRight;
    maxHeight += _style.marginTop + _style.marginBottom;

    return {
        static_cast<uint8_t>(totalWidth),
        static_cast<uint8_t>(maxHeight)
    };
}

bool HorizontalLayout::canExpandHorizontally() const
{
    if (_style.spacingMode != SpacingMode::Fixed)
        return true;

    for (size_t i = 0; i < EnChildCount(); i++)
        if (auto* w = EnChild(i))
            if (w->canExpandHorizontally())
                return true;

    return false;
}

bool HorizontalLayout::canExpandVertically() const
{
    for (size_t i = 0; i < EnChildCount(); i++)
        if (auto* w = EnChild(i))
            if (w->canExpandVertically())
                return true;

    return false;
}

} // namespace widgets
} // namespace ui