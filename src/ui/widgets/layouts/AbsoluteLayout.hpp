#pragma once

#include "../core/Widget.hpp"
#include "../core/Alignment.hpp"
#include <cstddef>

constexpr size_t MAX_CHILDREN = 16;

class AbsoluteLayout : public Widget
{
private:
    struct ChildLayout {
        Widget* widget;
        Rect box;
        Alignment align;
    };

public:
    AbsoluteLayout(Rect layoutBox);
    virtual ~AbsoluteLayout() = default;

    void addChild(Widget* widget, Rect box, Alignment align);
    void render(Renderer& r, Rect parentCanvas) override;
    Size measure() const override;

    size_t childCount() const override { return _count; }
    Widget* child(size_t index) const override;

private:
    ChildLayout _children[MAX_CHILDREN]{};
    size_t _count{0};
    Rect _layoutBox;
};
