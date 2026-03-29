#include "LinearLayout.hpp"

namespace ui {
namespace widgets {

Rect LinearLayout::applyMargins(Rect box) const
{
    return {
        static_cast<uint8_t>(box.x + _style.marginLeft),
        static_cast<uint8_t>(box.y + _style.marginTop),
        static_cast<uint8_t>(box.w - _style.marginLeft - _style.marginRight),
        static_cast<uint8_t>(box.h - _style.marginTop - _style.marginBottom)
    };
}

LinearLayout::SpacingInfo LinearLayout::computeSpacing(uint16_t availableSpace,
                                        uint16_t totalChildSize,
                                        size_t count) const
{
    if (count <= 1)
        return {static_cast<double>(_style.spacing), 0};

    int remaining = static_cast<int>(availableSpace) - static_cast<int>(totalChildSize);
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

std::vector<LinearLayout::ChildInfo>
LinearLayout::expandExpandableChildren(std::vector<ChildInfo> children,
                                           uint16_t availableSpace) const {

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

    int remainingWidth = availableSpace - fixedWidth - totalSpacing;

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
                                                    
std::vector<LinearLayout::LayoutItem> LinearLayout::computeLayout(Rect contentArea) const
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

            bool canExpandPrimary = (_primaryAxis == Axis::Horizontal) ? w->canExpandHorizontally() : w->canExpandVertically();
            bool canExpandSecondary = (_primaryAxis == Axis::Horizontal) ? w->canExpandVertically() : w->canExpandHorizontally();

            uint16_t primarySize = (_primaryAxis == Axis::Horizontal) ? s.w : s.h;
            uint16_t secondarySize = (_primaryAxis == Axis::Horizontal) ? 
                (canExpandSecondary ? contentArea.h : s.h) : 
                (canExpandSecondary ? contentArea.w : s.w);

            children.push_back({
                w,
                primarySize,
                primarySize,
                secondarySize,
                canExpandPrimary,
                false
            });
        }
    }

    uint16_t availableSpace = (_primaryAxis == Axis::Horizontal) ? contentArea.w : contentArea.h;
    uint16_t totalChildSize = 0;
    for (const auto& c : children)
        totalChildSize += c.width;  // width is primary size

    SpacingInfo spacing = {0, 0};
    if (!std::any_of(children.begin(), children.end(), [](const ChildInfo& c){ return c.expand; }))
    {
        spacing = computeSpacing(availableSpace, totalChildSize, children.size());
    }
    else
    {
        spacing = {
            static_cast<double>(_style.spacing),
            0.0
        };

        children = expandExpandableChildren(children, availableSpace);
    }

    int primaryPos = (_primaryAxis == Axis::Horizontal) ? contentArea.x : contentArea.y;
    primaryPos += spacing.around;

    double spacingAccumulatedError = 0.0;

    for (size_t i = 0; i < children.size(); i++)
    {
        auto& c = children[i];

        int secondaryPos = (_primaryAxis == Axis::Horizontal) ? contentArea.y : contentArea.x;

        if (_primaryAxis == Axis::Horizontal)
        {
            switch (_style.verticalAlign)
            {
                case VerticalAlignment::Middle:
                    secondaryPos += (contentArea.h - c.height) / 2;
                    break;
                case VerticalAlignment::Bottom:
                    secondaryPos += contentArea.h - c.height;
                    break;
                default:
                    break;
            }
        }
        else
        {
            switch (_style.horizontalAlign)
            {
                case HorizontalAlignment::Center:
                    secondaryPos += (contentArea.w - c.height) / 2;  // c.height is secondary size
                    break;
                case HorizontalAlignment::Right:
                    secondaryPos += contentArea.w - c.height;
                    break;
                default:
                    break;
            }
        }

        Rect rect;
        if (_primaryAxis == Axis::Horizontal)
        {
            rect = {
                static_cast<uint8_t>(primaryPos),
                static_cast<uint8_t>(secondaryPos),
                static_cast<uint8_t>(c.width),
                static_cast<uint8_t>(c.height)
            };
        }
        else
        {
            rect = {
                static_cast<uint8_t>(secondaryPos),
                static_cast<uint8_t>(primaryPos),
                static_cast<uint8_t>(c.height),  // width is secondary
                static_cast<uint8_t>(c.width)   // height is primary
            };
        }

        result.push_back({
            c.widget,
            rect
        });

        primaryPos += c.width;

        if (i + 1 < children.size())
        {
            double spacingRounded = std::round(spacing.between);
            primaryPos += spacingRounded;
            spacingAccumulatedError += spacing.between - spacingRounded;
        
            if (spacingAccumulatedError >= 0.5) {
                primaryPos += 1;
                spacingAccumulatedError -= 0.5;
            }
            else if (spacingAccumulatedError <= -0.5) {
                primaryPos -= 1;
                spacingAccumulatedError += 0.5;
            }
        }
    }

    return result;
}

void LinearLayout::render(Renderer& r, Rect canvasBox)
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

Size LinearLayout::measure() const
{
    uint16_t primaryTotal = 0;
    uint16_t secondaryMax = 0;

    const size_t count = EnChildCount();

    for (size_t i = 0; i < count; i++)
    {
        if (auto* w = EnChild(i))
        {
            Size s = w->measure();

            if (_primaryAxis == Axis::Horizontal)
            {
                primaryTotal += s.w;
                secondaryMax = std::max(secondaryMax, static_cast<uint16_t>(s.h));
            }
            else
            {
                primaryTotal += s.h;
                secondaryMax = std::max(secondaryMax, static_cast<uint16_t>(s.w));
            }
        }
    }

    if (count > 1 && _style.spacingMode == SpacingMode::Fixed)
        primaryTotal += (count - 1) * _style.spacing;

    uint16_t totalWidth = (_primaryAxis == Axis::Horizontal) ? primaryTotal : secondaryMax;
    uint16_t totalHeight = (_primaryAxis == Axis::Horizontal) ? secondaryMax : primaryTotal;

    totalWidth += _style.marginLeft + _style.marginRight;
    totalHeight += _style.marginTop + _style.marginBottom;

    return {
        static_cast<uint8_t>(totalWidth),
        static_cast<uint8_t>(totalHeight)
    };
}

bool LinearLayout::canExpandHorizontally() const
{
    if (_primaryAxis == Axis::Horizontal)
    {
        if (_style.spacingMode != SpacingMode::Fixed)
            return true;

        for (size_t i = 0; i < EnChildCount(); i++)
            if (auto* w = EnChild(i))
                if (w->canExpandHorizontally())
                    return true;

        return false;
    }
    else
    {
        for (size_t i = 0; i < EnChildCount(); i++)
            if (auto* w = EnChild(i))
                if (w->canExpandHorizontally())
                    return true;

        return false;
    }
}

bool LinearLayout::canExpandVertically() const
{
    if (_primaryAxis == Axis::Vertical)
    {
        if (_style.spacingMode != SpacingMode::Fixed)
            return true;

        for (size_t i = 0; i < EnChildCount(); i++)
            if (auto* w = EnChild(i))
                if (w->canExpandVertically())
                    return true;

        return false;
    }
    else
    {
        for (size_t i = 0; i < EnChildCount(); i++)
            if (auto* w = EnChild(i))
                if (w->canExpandVertically())
                    return true;

        return false;
    }
}

} // namespace widgets
} // namespace ui