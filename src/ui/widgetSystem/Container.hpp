#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Widget.hpp"
#include "SelectableWidget.hpp"
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

    virtual WidgetType type() const override { return WidgetType::Container; }

    void render(Renderer& r, Rect canvasBox) override
    {
        Rect newCanvas = {
            static_cast<uint8_t>(canvasBox.x + box().x),
            static_cast<uint8_t>(canvasBox.y + box().y),
            static_cast<uint8_t>(std::min(canvasBox.w, box().w)),
            static_cast<uint8_t>(std::min(canvasBox.h, box().h))
        };

        for (size_t i = 0; i < _count; i++)
        {
            if (_children[i] != nullptr)
                _children[i]->render(r, newCanvas);
        }
    }

    // Collect all selectable widgets recursively
    void collectSelectables(SelectableWidget* out[], size_t& count) const
    {
        for (size_t i = 0; i < _count; i++)
        {
            Widget* child = _children[i];
            if (child == nullptr) continue;

            if (child->type() == WidgetType::Selectable)
            {
                out[count++] = static_cast<SelectableWidget*>(child);
            }
            else if (child->type() == WidgetType::Container)
            {
                static_cast<Container*>(child)->collectSelectables(out, count);
            }
        }
    }

private:
    Widget* _children[MAX_CHILDREN]{ nullptr };
    size_t _count{ 0 };
};

#endif