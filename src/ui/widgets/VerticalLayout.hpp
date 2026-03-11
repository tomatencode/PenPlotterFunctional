#pragma once

#include "../widgetSystem/Widget.hpp"

constexpr size_t MAX_LAYOUT_CHILDREN = 16;

class VerticalLayout : public Widget
{
public:
    // Constructor: box + array of children
    VerticalLayout(Rect box, Alignment align, Widget* children[], size_t count);

    void render(Renderer& r, Rect canvasBox) override;
    Size measure() const override;

    virtual size_t childCount() const override { return _count; }
    virtual Widget* child(size_t index) const override;

protected:
    Widget* _children[MAX_LAYOUT_CHILDREN]{ nullptr };
    size_t _count{ 0 };
    
    // Calculate total height of all children
    uint16_t getTotalHeight() const;
};