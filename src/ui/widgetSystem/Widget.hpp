#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "../render/Render.hpp"
#include "box.hpp"
#include "Size.hpp"
#include "Alignment.hpp"

class Widget
{
public:
    Widget(Rect box, Alignment align) : _box(box), _align(align) {}

    virtual void render(Renderer& r, Rect canvasBox) = 0;

    virtual Size measure() const { return Size{_box.w, _box.h}; }

    virtual bool isSelectable() const { return false; }

    virtual size_t childCount() const { return 0; }
    virtual Widget* child(size_t) const { return nullptr; }

    Rect box() const { return _box; }
    Alignment align() const { return _align; }
protected:

Rect computeContentRect(Rect parentCanvas) const
    {
        Rect widgetBox = box();

        int absX = parentCanvas.x + widgetBox.x;
        int absY = parentCanvas.y + widgetBox.y;

        Size content = measure();

        // alignment inside widget box
        int startX = absX;
        int startY = absY;

        switch (_align.horizontal)
        {
            case HorizontalAlignment::Center:
                startX += (widgetBox.w - content.w) / 2;
                break;

            case HorizontalAlignment::Right:
                startX += widgetBox.w - content.w;
                break;

            default:
                break;
        }

        switch (_align.vertical)
        {
            case VerticalAlignment::Middle:
                startY += (widgetBox.h - content.h) / 2;
                break;

            case VerticalAlignment::Bottom:
                startY += widgetBox.h - content.h;
                break;

            default:
                break;
        }

        // clipping against parent canvas
        int remainingW = std::max(0, int(parentCanvas.w) - int(widgetBox.x));
        int remainingH = std::max(0, int(parentCanvas.h) - int(widgetBox.y));

        int w = std::min<int>(content.w, remainingW);
        int h = std::min<int>(content.h, remainingH);

        return Rect{
            static_cast<uint8_t>(startX),
            static_cast<uint8_t>(startY),
            static_cast<uint8_t>(w),
            static_cast<uint8_t>(h)
        };
    }
private:
    Rect _box;
    Alignment _align;
};

#endif