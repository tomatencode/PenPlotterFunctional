#include "CollectSelectables.hpp"

void collectSelectables(Widget* w, SelectableWidget* out[], size_t& count) {
    if (!w) return;

    if (w->isSelectable())
        out[count++] = static_cast<SelectableWidget*>(w);

    for (size_t i = 0; i < w->childCount(); i++)
        collectSelectables(w->child(i), out, count);
}