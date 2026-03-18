#pragma once

#include <memory>
#include <vector>

#include "Widget.hpp"
#include "LayoutStyle.hpp"

// Maximum number of children for layouts
constexpr int MAX_LAYOUT_CHILDREN = 16;

class LayoutWidget : public Widget
{
public:
    LayoutWidget(Widget* children[], size_t count, const LayoutStyle& style = LayoutStyle())
        : _style(style)
    {
        setChildren(children, count);
    }

    virtual size_t childCount() const override { return _count; }
    virtual Widget* child(size_t index) const override {
        if (index < _count)
            return _children[index];
        return nullptr;
    }

    // Owning constructor: takes ownership of the widgets
    LayoutWidget(std::vector<std::unique_ptr<Widget>>&& ownedChildren, const LayoutStyle& style = LayoutStyle())
        : _style(style), _ownedChildren(std::move(ownedChildren))
    {
        _children.reserve(_ownedChildren.size());
        for (auto& child : _ownedChildren)
        {
            if (child)
                _children.push_back(child.get());
        }
        _count = _children.size();
    }

    const LayoutStyle& style() const { return _style; }

protected:
    void setChildren(Widget* children[], size_t count)
    {
        _children.clear();
        _count = 0;
        for (size_t i = 0; i < count && i < MAX_LAYOUT_CHILDREN; i++)
        {
            if (children[i] != nullptr)
                _children.push_back(children[i]);
        }
        _count = _children.size();
    }

private:
    std::vector<std::unique_ptr<Widget>> _ownedChildren;
    std::vector<Widget*> _children;
    size_t _count{0};

    LayoutStyle _style;
};