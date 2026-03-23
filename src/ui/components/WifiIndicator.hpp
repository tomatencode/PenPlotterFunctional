#pragma once

#include <memory>

#include "../framework/widgets/core/Widget.hpp"
#include "../framework/widgets/core/Container.hpp"
#include "../framework/widgets/leaves/Label.hpp"

namespace ui {
namespace components {

class WifiIndicator: public ui::widgets::Container
{
public:
    WifiIndicator();
};

} // namespace components
} // namespace ui