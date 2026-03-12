#pragma once

#include "../../render/Render.hpp"
#include "box.hpp"
#include "Size.hpp"

class Widget
{
public:
    Widget() {}
    
    // For leaf widgets that have a size
    Widget(uint8_t w, uint8_t h) : _w(w), _h(h) {}

    virtual void render(Renderer& r, Rect canvasBox) = 0;

    virtual Size measure() const { return Size{_w, _h}; }

    virtual bool isSelectable() const { return false; }

    virtual size_t childCount() const { return 0; }
    virtual Widget* child(size_t) const { return nullptr; }

protected:
    uint8_t _w = 0;
    uint8_t _h = 0;
};