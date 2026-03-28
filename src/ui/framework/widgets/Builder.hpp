#pragma once

#include <memory>
#include <utility>
#include <concepts>

#include "core/Widget.hpp"

namespace ui {
namespace widgets {

// Helper to create a heap-allocated widget (owning pointer)
// Usage: widgets::make_widget<widgets::Label>("Hello")
template <typename T>
concept WidgetDerived = std::is_base_of_v<Widget, T>;

template <WidgetDerived T, typename... Args>
requires std::constructible_from<T, Args...>
std::unique_ptr<Widget> make_widget(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

} // namespace widgets
} // namespace ui
