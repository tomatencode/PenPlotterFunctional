#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/Container.hpp"

namespace ui {
namespace components {

class HeaderLine: public ui::widgets::Container
{
public:
    HeaderLine(String headerText,
               std::function<bool()> WifiStatusProvider,
               std::function<void()> onBackPress = nullptr);
};

} // namespace components
} // namespace ui