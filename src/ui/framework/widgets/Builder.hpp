#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "core/Widget.hpp"
#include "layouts/HorizontalLayout.hpp"
#include "layouts/ScrollableVerticalLayout.hpp"
#include "layouts/VerticalLayout.hpp"

namespace ui {
namespace widgets {

// Helper to create a heap-allocated widget (owning pointer)
// Usage: make_widget<LabelWidget>("Hello")
template <typename T, typename... Args>
std::unique_ptr<Widget> make_widget(Args&&... args)
{
    return std::unique_ptr<Widget>(new T(std::forward<Args>(args)...));
}

// C++17 helper to compose a vector of children without copy-constructing unique_ptr.
// Usage: make_children(make_widget<LabelWidget>("Hello"), make_widget<ButtonWidget>(...));
template <typename... Children>
std::vector<std::unique_ptr<Widget>> make_children(Children&&... children)
{
    std::vector<std::unique_ptr<Widget>> v;
    v.reserve(sizeof...(Children));
    (void)std::initializer_list<int>{(v.push_back(std::forward<Children>(children)), 0)...};
    return v;
}

// C++17 helper to construct layout widget while handling style + children vector.
// Assumes LayoutType has constructor: LayoutType(const Style&, std::vector<std::unique_ptr<Widget>>&&)
template <typename LayoutType, typename Style, typename... Children>
std::unique_ptr<Widget> make_layout(const Style& style, Children&&... children)
{
    return std::unique_ptr<Widget>(new LayoutType(style, make_children(std::forward<Children>(children)...)));
}

} // namespace widgets
} // namespace ui
