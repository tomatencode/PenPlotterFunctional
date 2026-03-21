#include "WifiIndicator.hpp"
#include "webInterface/WebInterface.hpp"
#include "../framework/text/textSources/FunctionGlyph.hpp"
#include "../framework/text/customChars.hpp"
#include "../framework/widgets/Builder.hpp"

namespace ui {
namespace components {

using namespace widgets;


WifiIndicator::WifiIndicator()
    : ContainerWidget(
        widgets::make_widget<LabelWidget>(
                std::unique_ptr<FunctionGlyph>(
                    new FunctionGlyph([]() -> const Glyph* {
                        static Glyph glyphs[2] = {GLYPH_SPACE, GLYPH_TERMINATOR};
                        glyphs[0] = false ? Glyph(CustomChar::WifiSymbol) : Glyph(CustomChar::NoWifiSymbol); // TODO: replace 'false' with actual wifi status
                        return glyphs;
                    })
                )
            )
        )
{
}

} // namespace components
} // namespace ui