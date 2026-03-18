#include "CollectSelectables.hpp"
#include "ui/framework/focusManager/FocusManager.hpp"

namespace ui {
namespace widgets {

void collectSelectables(Widget* w, SelectableWidget* out[], size_t& count) {
    if (!w || count >= MAX_SELECTABLE_WIDGETS) return;

    if (w->isSelectable())
        out[count++] = static_cast<SelectableWidget*>(w);

    for (size_t i = 0; i < w->childCount(); i++)
        collectSelectables(w->child(i), out, count);
}

} // namespace widgets
} // namespace ui
