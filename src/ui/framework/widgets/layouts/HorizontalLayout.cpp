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

HorizontalLayout::Metrics HorizontalLayout::computeMetrics() const
{
    Metrics m;

    for (size_t i = 0; i < EnChildCount(); i++)
    {
        if (auto* w = EnChild(i)) {
            Size s = w->measure();
            m.totalWidth += s.w;
            m.maxHeight = std::max(m.maxHeight, static_cast<uint16_t>(s.h));
        }
    }

    return m;
}

double HorizontalLayout::computeSpacing(uint16_t availableWidth, uint16_t totalChildWidth, size_t count) const
{
    if (count <= 1)
        return (_style.spacingMode == SpacingMode::Fixed) ? _style.spacing : 0;

    int remaining = static_cast<int>(availableWidth) - static_cast<int>(totalChildWidth);
    remaining = std::max(0, remaining);

    switch (_style.spacingMode)
    {
        case SpacingMode::Even:
            return static_cast<double>(remaining) / (count + 1);

        case SpacingMode::SpaceBetween:
            return static_cast<double>(remaining) / (count - 1);

        case SpacingMode::SpaceAround:
            return static_cast<double>(remaining) / count;

        default:
            return _style.spacing;
    }
}

std::vector<HorizontalLayout::LayoutItem>
HorizontalLayout::computeLayout(Rect contentArea) const
{
    std::vector<LayoutItem> result;

    const size_t count = EnChildCount();
    if (count == 0) return result;

    Metrics m = computeMetrics();
    double spacing = computeSpacing(contentArea.w, m.totalWidth, count);

    double spacingError = 0.0;
    int currentX = contentArea.x;

    // initial offset
    if (_style.spacingMode == SpacingMode::Even) {
        int s = std::round(spacing);
        currentX += s;
        spacingError += spacing - s;
    }
    else if (_style.spacingMode == SpacingMode::SpaceAround) {
        int s = std::round(spacing / 2);
        currentX += s;
        spacingError += (spacing / 2) - s;
    }

    for (size_t i = 0; i < count; i++)
    {
        Widget* w = EnChild(i);
        if (!w) continue;

        Size minSize = w->measure();

        uint16_t width = minSize.w;
        uint16_t height = w->canExpandVertically() ? contentArea.h : minSize.h;

        int childY = contentArea.y;

        switch (_style.verticalAlign)
        {
            case VerticalAlignment::Middle:
                childY += (contentArea.h - height) / 2;
                break;
            case VerticalAlignment::Bottom:
                childY += contentArea.h - height;
                break;
            default:
                break;
        }

        Rect rect {
            static_cast<uint8_t>(currentX),
            static_cast<uint8_t>(childY),
            width,
            height
        };

        result.push_back({w, rect});

        int s = std::round(spacing);
        currentX += width + s;
        spacingError += spacing - s;

        if (spacingError >= 0.5) {
            currentX += 1;
            spacingError -= 1.0;
        }
        else if (spacingError <= -0.5) {
            currentX -= 1;
            spacingError += 1.0;
        }
    }

    return result;
}

Size HorizontalLayout::measure() const
{
    auto m = computeMetrics();

    uint16_t width = m.totalWidth;
    uint16_t height = m.maxHeight;

    if (EnChildCount() > 1 && _style.spacingMode == SpacingMode::Fixed)
        width += (EnChildCount() - 1) * _style.spacing;

    width += _style.marginLeft + _style.marginRight;
    height += _style.marginTop + _style.marginBottom;

    return Size{
        static_cast<uint8_t>(width),
        static_cast<uint8_t>(height)
    };
}

void HorizontalLayout::render(Renderer& r, Rect canvasBox)
{
    if (EnChildCount() == 0) return;

    Rect content = applyMargins(canvasBox);

    auto layout = computeLayout(content);

    for (auto& item : layout)
    {
        if (item.widget)
            item.widget->render(r, item.rect);
    }
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