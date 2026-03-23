#include "WifiIndicator.hpp"
#include "webInterface/WebInterface.hpp"
#include "../framework/text/customChars.hpp"
#include "../framework/widgets/Builder.hpp"
#include "../framework/text/GlyphString.hpp"

namespace ui {
namespace components {

WifiIndicator::WifiIndicator()
    : Container(
        widgets::make_widget<widgets::Label>([]() -> const GlyphString {
            return false ? GlyphString({Glyph(CustomChar::WifiSymbol)}) : GlyphString({Glyph(CustomChar::NoWifiSymbol)}); // TODO: replace 'false' with actual wifi status
        })
    )
{
}

} // namespace components
} // namespace ui