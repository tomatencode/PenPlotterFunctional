#pragma once

#include "../../render/Render.hpp"
#include "box.hpp"
#include "Size.hpp"

class Widget
{
public:
    Widget() {}

    virtual void render(Renderer& r, Rect canvasBox) = 0;

    virtual Size measure() const;

    virtual bool isSelectable() const { return false; }

    virtual size_t childCount() const { return 0; }
    virtual Widget* child(size_t) const { return nullptr; }
};