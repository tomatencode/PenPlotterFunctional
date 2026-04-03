#pragma once

#include "Glyph.hpp"
#include <cstdint>

namespace ui {

inline Glyph WifiSymbolGlyph{0};
inline Glyph NoWifiSymbolGlyph{1};

extern uint8_t* CUSTOM_CHAR_MAP[];

} // namespace ui