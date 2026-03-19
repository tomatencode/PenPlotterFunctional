#pragma once

#include <vector>

#include "Widget.hpp"
#include "SelectableWidget.hpp"

namespace ui {
namespace widgets {

void collectSelectables(Widget* w, std::vector<SelectableWidget*>& out);

} // namespace widgets
} // namespace ui
