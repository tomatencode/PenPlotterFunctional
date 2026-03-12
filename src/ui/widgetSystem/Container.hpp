#pragma once

#include "Widget.hpp"
#include "Alignment.hpp"
#include "WidgetUtils.hpp"
#include <cstddef>

constexpr size_t MAX_CHILDREN = 16;

class Container : public Widget
{
protected:
    struct ChildLayout {
        Widget* widget;
        Rect box;
        Alignment align;
    };

public:
    Container(Rect containerBox)
        : _containerBox(containerBox), _count(0)
    {
    }

    virtual ~Container() = default;

    // Add a child with absolute positioning and alignment
    void addChild(Widget* widget, Rect box, Alignment align)
    {
        if (_count < MAX_CHILDREN && widget)
        {
            _children[_count] = {widget, box, align};
            _count++;
        }
    }

    void render(Renderer& r, Rect parentCanvas) override
    {
        // Compute container's absolute position
        int absX = parentCanvas.x + _containerBox.x;
        int absY = parentCanvas.y + _containerBox.y;

        // Create container's draw rect (clipped to parent)
        Rect containerDrawRect{
            static_cast<uint8_t>(absX),
            static_cast<uint8_t>(absY),
            _containerBox.w,
            _containerBox.h
        };

        // Render each child at its specified position
        for (size_t i = 0; i < _count; i++)
        {
            if (_children[i].widget == nullptr)
                continue;

            Size childSize = _children[i].widget->measure();
            
            // Compute child's position with alignment inside its box
            Rect drawRect = computeContentAlignment(
                _children[i].box,
                _children[i].align,
                childSize,
                containerDrawRect
            );

            _children[i].widget->render(r, drawRect);
        }
    }

    Size measure() const override
    {
        return Size{_containerBox.w, _containerBox.h};
    }

    size_t childCount() const override { return _count; }
    Widget* child(size_t index) const override
    {
        if (index < _count)
            return _children[index].widget;
        return nullptr;
    }

protected:
    Rect getContainerBox() const { return _containerBox; }
    const ChildLayout& getChildLayout(size_t index) const { return _children[index]; }
    size_t getChildCount() const { return _count; }

private:
    ChildLayout _children[MAX_CHILDREN]{};
    size_t _count{0};
    Rect _containerBox;
};
