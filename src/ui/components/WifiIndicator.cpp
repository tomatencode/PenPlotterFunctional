#include "WifiIndicator.hpp"
#include "webInterface/WebInterface.hpp"
#include "../framework/text/textSources/FunctionGlyph.hpp"

extern WebInterface webInterface;

using namespace ui;

namespace components {

WifiIndicator::WifiIndicator()
    : Widget()
    , _label(std::unique_ptr<ui::widgets::LabelWidget>(
          new ui::widgets::LabelWidget(
              std::unique_ptr<ui::FunctionGlyph>(
                  new ui::FunctionGlyph([]() -> const Glyph* {
                      static Glyph glyphs[2] = {GLYPH_SPACE, GLYPH_TERMINATOR};
                      glyphs[0] = webInterface.isWiFiConnected() ? Glyph('W') : GLYPH_SPACE;
                      return glyphs;
                  })
              )
          )
      ))
{
}

void WifiIndicator::render(Renderer& r, widgets::Rect canvasBox)
{
    if (_label)
        _label->render(r, canvasBox);
}

widgets::Size WifiIndicator::measure() const
{
    return _label ? _label->measure() : widgets::Size{0, 0};
}

} // namespace components