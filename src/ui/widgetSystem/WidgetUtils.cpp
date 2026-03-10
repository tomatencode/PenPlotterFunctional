#include "WidgetUtils.hpp"

void collectSelectables(Widget* w, SelectableWidget* out[], size_t& count) {
    if (!w) return;

    if (w->isSelectable())
        out[count++] = static_cast<SelectableWidget*>(w);

    for (size_t i = 0; i < w->childCount(); i++)
        collectSelectables(w->child(i), out, count);
}

Rect computeContentAlignment(Rect widgetBox, Alignment align, Size contentSize, Rect parentCanvas) {

    int absX = parentCanvas.x + widgetBox.x;
    int absY = parentCanvas.y + widgetBox.y;

    // alignment inside widget box
    int startX = absX;
    int startY = absY;

    switch (align.horizontal)
    {
        case HorizontalAlignment::Center:
            startX += (widgetBox.w - contentSize.w) / 2;
            break;

        case HorizontalAlignment::Right:
            startX += widgetBox.w - contentSize.w;
            break;

        default:
            break;
    }

    switch (align.vertical)
    {
        case VerticalAlignment::Middle:
            startY += (widgetBox.h - contentSize.h) / 2;
            break;

        case VerticalAlignment::Bottom:
            startY += widgetBox.h - contentSize.h;
            break;

        default:
            break;
    }

    // clipping against parent canvas
    int remainingW = std::max(0, int(parentCanvas.w) - int(widgetBox.x));
    int remainingH = std::max(0, int(parentCanvas.h) - int(widgetBox.y));

    int w = std::min<int>(contentSize.w, remainingW);
    int h = std::min<int>(contentSize.h, remainingH);

    return Rect{
        static_cast<uint8_t>(startX),
        static_cast<uint8_t>(startY),
        static_cast<uint8_t>(w),
        static_cast<uint8_t>(h)
    };
}