#include "WifiIndicator.hpp"
#include "webInterface/WebInterface.hpp"
#include "../framework/text/textSources/FunctionGlyph.hpp"
#include "../framework/text/customChars.hpp"

extern WebInterface webInterface;

namespace ui {
namespace components {

using namespace widgets;


WifiIndicator::WifiIndicator()
    : Widget()
    , _label(std::unique_ptr<LabelWidget>(
          new LabelWidget(
              std::unique_ptr<FunctionGlyph>(
                  new FunctionGlyph([]() -> const Glyph* {
                      static Glyph glyphs[2] = {GLYPH_SPACE, GLYPH_TERMINATOR};
                      glyphs[0] = webInterface.isWiFiConnected() ? Glyph(CustomChar::WifiSymbol) : Glyph(CustomChar::NoWifiSymbol);
                      return glyphs;
                  })
              )
          )
      ))
{
}

void WifiIndicator::render(Renderer& r, Rect canvasBox)
{
    if (_label)
        _label->render(r, canvasBox);
}

Size WifiIndicator::measure() const
{
    return _label ? _label->measure() : Size{0, 0};
}

} // namespace components
} // namespace ui