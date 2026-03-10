#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Widget.hpp"
#include "SelectableWidget.hpp"
#include "WidgetUtils.hpp"
#include <algorithm>
#include <cstddef>

constexpr size_t MAX_CHILDREN = 16; // maximum widgets per container

class Container : public Widget
{
public:
    // Constructor: box + number of children
    Container(Rect box, Alignment align, Widget* children[], size_t count)
        : Widget(box, align), _count(count)
    {
        for (size_t i = 0; i < _count && i < MAX_CHILDREN; i++)
            _children[i] = children[i];
    }

    void render(Renderer& r, Rect canvasBox) override
    {
        Rect drawRect = computeContentAlignment(canvasBox, align(), measure(), canvasBox);
        
        for (size_t i = 0; i < _count; i++)
        {
            if (_children[i] != nullptr)
                _children[i]->render(r, drawRect);
        }
    }

    virtual size_t childCount() const override { return _count; }
    virtual Widget* child(size_t index) const override {
        if (index < _count)
            return _children[index];
        return nullptr;
    }

private:
    Widget* _children[MAX_CHILDREN]{ nullptr };
    size_t _count{ 0 };
};

#endif