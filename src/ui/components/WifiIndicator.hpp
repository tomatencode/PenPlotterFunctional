#pragma once

#include <memory>
#include <functional>

#include "ui/framework/widgets/core/Widget.hpp"
#include "ui/framework/widgets/core/Container.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"

namespace ui {
namespace components {

struct WifiIndicatorProps {
    std::function<bool()> wifiStatusProvider;
};

class WifiIndicator: public ui::widgets::Container
{
public:
    WifiIndicator(WifiIndicatorProps props)
    : Container(
        std::make_unique<widgets::Label>([props]() -> const GlyphString {
            return props.wifiStatusProvider() ? customChars::WifiSymbol : customChars::NoWifiSymbol;
        })
    )
{
}
};

} // namespace components
} // namespace ui