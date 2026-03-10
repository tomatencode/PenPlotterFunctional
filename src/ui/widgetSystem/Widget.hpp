#ifndef WIDGET_HPP
#define WIDGET_HPP

#include "../render/Render.hpp"
#include "box.hpp"
#include "Size.hpp"
#include "Alignment.hpp"
#include "WidgetTypes.hpp"

class Widget
{
public:
    Widget(Rect box, Alignment align) : _box(box), _align(align) {}

    virtual void render(Renderer& r, Rect canvasBox) = 0;

    virtual Size measure() const { return Size{_box.w, _box.h}; }

    virtual WidgetType type() const { return WidgetType::Base; }
    Rect box() const { return _box; }
    Alignment align() const { return _align; }
private:
    Rect _box;
    Alignment _align;
};

#endif