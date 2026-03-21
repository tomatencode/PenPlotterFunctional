#pragma once

#include <memory>

#include "../framework/widgets/core/Widget.hpp"
#include "../framework/widgets/core/ContainerWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"

namespace ui {
namespace components {

class WifiIndicator: public ui::widgets::ContainerWidget
{
public:
    WifiIndicator();
};

} // namespace components
} // namespace ui