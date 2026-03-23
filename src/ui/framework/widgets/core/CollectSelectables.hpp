#pragma once

#include <vector>

#include "Widget.hpp"
#include "SelectableWidget.hpp"

namespace ui {
namespace widgets {

void collectSelectables(Widget* w, std::vector<Selectable*>& out) {
    if (!w) return;

    if (w->isSelectable())
        out.push_back(static_cast<Selectable*>(w));

    for (size_t i = 0; i < w->childCount(); i++)
        collectSelectables(w->child(i), out);
}

} // namespace widgets
} // namespace ui
