#pragma once

#include "Widget.hpp"
#include "LayoutStyle.hpp"

// Maximum number of children for layouts
constexpr int MAX_LAYOUT_CHILDREN = 16;

class LayoutWidget : public Widget
{
public:
    LayoutWidget(Widget* children[], size_t count, const LayoutStyle& style = LayoutStyle())
        : _count(0), _style(style)
    {
        setChildren(children, count);
    }

    virtual size_t childCount() const override { return _count; }
    virtual Widget* child(size_t index) const override {
        if (index < _count)
            return _children[index];
        return nullptr;
    }

    const LayoutStyle& style() const { return _style; }

protected:
    void setChildren(Widget* children[], size_t count)
    {
        _count = 0;
        for (size_t i = 0; i < count && i < MAX_LAYOUT_CHILDREN; i++)
        {
            if (children[i] != nullptr)
                _children[_count++] = children[i];
        }
    }

private:
    Widget* _children[MAX_LAYOUT_CHILDREN]{nullptr};
    size_t _count{0};

    LayoutStyle _style;
};