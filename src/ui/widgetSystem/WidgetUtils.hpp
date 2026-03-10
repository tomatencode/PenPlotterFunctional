#ifndef WIDGET_UTILS_HPP
#define WIDGET_UTILS_HPP

#include "Widget.hpp"
#include "SelectableWidget.hpp"

void collectSelectables(Widget* w, SelectableWidget* out[], size_t& count);

Rect computeContentAlignment(Rect widgetBox, Alignment align, Size contentSize, Rect parentCanvas);

#endif