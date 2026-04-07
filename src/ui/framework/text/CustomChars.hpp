#pragma once

#include "Glyph.hpp"
#include <cstdint>

namespace ui {
namespace customChars {

inline Glyph WifiSymbol{0};
inline Glyph NoWifiSymbol{1};
inline Glyph TextEditorEnter{2};
inline Glyph TextEditorDelete{3};
inline Glyph DegSymbol{4};
inline Glyph Ellipsis{7};

extern uint8_t* CUSTOM_CHAR_MAP[];
    
} // namespace customChars
} // namespace ui