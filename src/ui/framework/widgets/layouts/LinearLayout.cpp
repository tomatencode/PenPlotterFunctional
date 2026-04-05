#include "LinearLayout.hpp"

namespace ui {
namespace widgets {

Box LinearLayout::applyMargins(Box box) const
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

// ---------- Axis helpers ----------

uint16_t LinearLayout::primarySize(Size s) const {
    return (_style.axis == Axis::Horizontal) ? s.w : s.h;
}

uint16_t LinearLayout::secondarySize(Size s) const {
    return (_style.axis == Axis::Horizontal) ? s.h : s.w;
}

uint16_t LinearLayout::PrimaryPos(Box r) const {
    return (_style.axis == Axis::Horizontal) ? r.x : r.y;
}
uint16_t LinearLayout::SecondaryPos(Box r) const {
    return (_style.axis == Axis::Horizontal) ? r.y : r.x;
}

uint16_t LinearLayout::availablePrimarySpace(Box r) const {
    return (_style.axis == Axis::Horizontal) ? r.w : r.h;
}

uint16_t LinearLayout::availableSecondarySpace(Box r) const {
    return (_style.axis == Axis::Horizontal) ? r.h : r.w;
}

// ---------- Spacing ----------

LinearLayout::Spacing
LinearLayout::computeSpacing(uint16_t available, uint16_t total, size_t childCount) const
{
    if (childCount <= 1)
        return {double(_style.spacing), 0};

    uint16_t remaining = std::max(0, static_cast<int>(available) - static_cast<int>(total));

    switch (_style.spacingMode)
    {
        case SpacingMode::Even:
            return {static_cast<double>(remaining) / (childCount + 1),
                    static_cast<double>(remaining) / (childCount + 1)};

        case SpacingMode::SpaceBetween:
            return {static_cast<double>(remaining) / (childCount - 1), 0};

        case SpacingMode::SpaceAround:
            return {static_cast<double>(remaining) / childCount,
                    static_cast<double>(remaining) / (2 * childCount)};

        default:
            return {double(_style.spacing), 0};
    }
}

// ---------- Expansion ----------

void LinearLayout::distributeExpansion(std::vector<ChildInfo>& children,
                                       uint16_t available) const
{
    uint16_t fixedSize = 0;
    uint16_t expandableCount = 0;

    for (auto& c : children) {
        if (c.canExpand) expandableCount++;
        else fixedSize += c.minPrimarySize;
    }

    if (expandableCount == 0) return;


    uint16_t totalSpacing = _style.spacing * (children.size() - 1);
    uint16_t remainingSpace = std::max(
        0,
        static_cast<int>(available) - static_cast<int>(fixedSize) - static_cast<int>(totalSpacing)
    );

    uint16_t targetSize = remainingSpace / expandableCount;

    // solve for equal expansion, while respecting minimum sizes
    bool changed = true;
    while (changed && expandableCount > 0)
    {
        changed = false;

        for (auto& child : children)
        {
            if (!child.canExpand || child.expansionLocked) continue;

            if (child.minPrimarySize > targetSize)
            {
                child.finalPrimarySize = child.minPrimarySize;
                child.expansionLocked = true;

                remainingSpace -= child.finalPrimarySize;
                expandableCount--;

                changed = true;
            }
        }

        if (expandableCount > 0)
            targetSize = remainingSpace / expandableCount;
    }

    for (auto& c : children)
        if (c.canExpand && !c.expansionLocked)
            c.finalPrimarySize = targetSize;
}

std::optional<Box> LinearLayout::computeChildRect(const ChildInfo& c, Box content, uint16_t childStart) const
{
    uint16_t start = PrimaryPos(content);
    uint16_t end = start + availablePrimarySpace(content);

    uint16_t childEnd = childStart + c.finalPrimarySize;

    // Child is at least partially visible - clamp to visible bounds
    uint16_t clampedStart = std::max(childStart, start);
    uint16_t clampedEnd = std::min(childEnd, end);
    uint16_t clampedSize = clampedEnd - clampedStart;

    if (clampedSize <= 0)
        return std::nullopt;

    // Calculate secondary axis position
    uint16_t secondaryPos = SecondaryPos(content);

    if (_style.axis == Axis::Horizontal)
    {
        if (_style.verticalAlign == VerticalAlignment::Middle)
            secondaryPos += (content.h - c.secondarySize) / 2;
        else if (_style.verticalAlign == VerticalAlignment::Bottom)
            secondaryPos += content.h - c.secondarySize;
    }
    else
    {
        if (_style.horizontalAlign == HorizontalAlignment::Center)
            secondaryPos += (content.w - c.secondarySize) / 2;
        else if (_style.horizontalAlign == HorizontalAlignment::Right)
            secondaryPos += content.w - c.secondarySize;
    }

    if (_style.axis == Axis::Horizontal)
    {
        return std::make_optional(Box{
            static_cast<uint16_t>(clampedStart),
            static_cast<uint16_t>(secondaryPos),
            static_cast<uint16_t>(clampedSize),
            static_cast<uint16_t>(c.secondarySize)
        });
    }
    else
    {
        return std::make_optional(Box{
            static_cast<uint16_t>(secondaryPos),
            static_cast<uint16_t>(clampedStart),
            static_cast<uint16_t>(c.secondarySize),
            static_cast<uint16_t>(clampedSize)
        });
    }
}

// ---------- Layout ----------

std::vector<LinearLayout::LayoutItem>
LinearLayout::computeLayout(Box content) const
{
    std::vector<LayoutItem> result;

    const size_t childCount = getChildCount();
    if (childCount == 0) return result;

    std::vector<ChildInfo> children;
    children.reserve(childCount);

    // 1. Collect children
    for (size_t i = 0; i < childCount; i++)
    {
        if (auto* child = getChild(i))
        {
            Size s = child->measure();

            bool expandPrimary = (_style.axis == Axis::Horizontal)
                ? child->canExpandHorizontally()
                : child->canExpandVertically();

            bool expandSecondary = (_style.axis == Axis::Horizontal)
                ? child->canExpandVertically()
                : child->canExpandHorizontally();

            uint16_t primSize = primarySize(s);
            uint16_t secSize = expandSecondary
                ? availableSecondarySpace(content)
                : std::min(secondarySize(s), availableSecondarySpace(content));

            children.push_back({child, primSize, primSize, secSize, expandPrimary});
        }
    }

    // 2. Compute total size
    uint16_t totalOccupiedSpace = 0;
    for (auto& c : children) totalOccupiedSpace += c.finalPrimarySize;

    uint16_t availableSpace = availablePrimarySpace(content);

    // 3. Expansion OR spacing
    bool hasExpandeble = std::any_of(children.begin(), children.end(),
                                 [](auto& c){ return c.canExpand; });

    Spacing spacing;
    if (hasExpandeble) {
        distributeExpansion(children, availableSpace);
        spacing = {static_cast<double>(_style.spacing), 0};
    }
    else {
        spacing = computeSpacing(availableSpace, totalOccupiedSpace, childCount);
    }

    // 4. Positioning
    uint16_t start = PrimaryPos(content);

    uint16_t pos = start + spacing.leading;

    double spacingError = 0;

    for (size_t i = 0; i < childCount; i++)
    {
        auto& child = children[i];

        uint16_t childStart = pos;
        auto rect = computeChildRect(child, content, childStart);

        if (rect.has_value())
            result.push_back({child.widget, rect.value()});

        
        // Advance position along primary axis
        pos += child.finalPrimarySize;
        if (pos > start + availableSpace)
            break; // no more space for additional children

        // Add spacing between children (if not the last child)
        if (i + 1 < childCount)
        {
            uint16_t spacingAmount = std::round(spacing.between);
            pos += spacingAmount;

            spacingError += spacing.between - spacingAmount;

            if (spacingError >= 0.5) { pos++; spacingError -= 1.0; }
            else if (spacingError <= -0.5) { pos--; spacingError += 1.0; }
        }
    }

    return result;
}

// ---------- Public API ----------

void LinearLayout::render(Renderer& r, Box canvasBox)
{
    if (getChildCount() == 0 || canvasBox.w == 0 || canvasBox.h == 0)
        return;

    Box content = applyMargins(canvasBox);
    auto layout = computeLayout(content);

    for (auto& item : layout)
        if (item.widget)
            item.widget->render(r, item.rect);
}

Size LinearLayout::measure() const
{
    uint16_t primary = 0;
    uint16_t secondary = 0;

    for (size_t i = 0; i < getChildCount(); i++)
    {
        if (auto* w = getChild(i))
        {
            Size s = w->measure();
            primary += primarySize(s);
            secondary = std::max(secondary, secondarySize(s));
        }
    }

    if (getChildCount() > 1 && _style.spacingMode == SpacingMode::Fixed)
        primary += (getChildCount() - 1) * _style.spacing;

    uint16_t w = (_style.axis == Axis::Horizontal) ? primary : secondary;
    uint16_t h = (_style.axis == Axis::Horizontal) ? secondary : primary;

    w += _style.marginLeft + _style.marginRight;
    h += _style.marginTop + _style.marginBottom;

    return {w, h};
}

bool LinearLayout::canExpandHorizontally() const
{
    if (_style.axis == Axis::Horizontal && _style.spacingMode != SpacingMode::Fixed)
        return true;

    for (size_t i = 0; i < getChildCount(); i++)
        if (auto* w = getChild(i))
            if (w->canExpandHorizontally())
                return true;

    return false;
}

bool LinearLayout::canExpandVertically() const
{
    if (_style.axis == Axis::Vertical && _style.spacingMode != SpacingMode::Fixed)
        return true;

    for (size_t i = 0; i < getChildCount(); i++)
        if (auto* w = getChild(i))
            if (w->canExpandVertically())
                return true;

    return false;
}

} // namespace widgets
} // namespace ui
