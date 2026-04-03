#include "WifiIndicator.hpp"
#include "webInterface/WebInterface.hpp"
#include "../framework/text/customChars.hpp"
#include "../framework/text/GlyphString.hpp"

namespace ui {
namespace components {

WifiIndicator::WifiIndicator(std::function<bool()> wifiStatusProvider)
    : Container(
        std::make_unique<widgets::Label>([wifiStatusProvider]() -> const GlyphString {
            return wifiStatusProvider() ? WifiSymbolGlyph : NoWifiSymbolGlyph;
        })
    )
{
}

} // namespace components
} // namespace ui