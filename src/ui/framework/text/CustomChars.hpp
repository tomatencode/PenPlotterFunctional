#pragma once

#include "Glyph.hpp"
#include <cstdint>

namespace ui {

inline Glyph WifiSymbolGlyph{0};
inline Glyph NoWifiSymbolGlyph{1};
inline Glyph TextEditorEnter{2};
inline Glyph TextEditorDelete{3};
inline Glyph DegSymbol{4};
inline Glyph Ellipsis{7};

extern uint8_t* CUSTOM_CHAR_MAP[];

} // namespace ui