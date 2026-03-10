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
private:
    Rect _box;
    Alignment _align;
};

#endif