#pragma once

#include "Widget.hpp"
#include "SelectableWidget.hpp"

namespace ui {
namespace widgets {

void collectSelectables(Widget* w, SelectableWidget* out[], size_t& count);

} // namespace widgets
} // namespace ui
