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

} // namespace widgets
} // namespace ui
