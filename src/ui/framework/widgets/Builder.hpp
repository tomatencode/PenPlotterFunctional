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

// Generic builder for any layout type that matches the constructor signature
// Layout must accept (std::vector<std::unique_ptr<Widget>>&&, const LayoutStyle&)
// Usage: make_layout<VerticalLayout>(make_widget<LabelWidget>("A"), ...)
template <typename Layout, typename... Children>
std::unique_ptr<Widget> make_layout(const LayoutStyle& style, Children&&... children)
{
    std::vector<std::unique_ptr<Widget>> list;
    list.reserve(sizeof...(Children));
    using expander = int[];
    (void)expander{0, ( (void)list.emplace_back(std::forward<Children>(children)), 0)... };
    return std::unique_ptr<Widget>(new Layout(std::move(list), style));
}

} // namespace widgets
} // namespace ui
