#pragma once

#include <cstdint>

namespace ui {

enum CustomChar
{
    WifiSymbol = 0,
    NoWifiSymbol = 1,
    CollapsibleExpanded = 2,
    CustomChar_4 = 3,
    CustomChar_5 = 4,
    CustomChar_6 = 5,
    CustomChar_7 = 6,
    CustomChar_8 = 7
};

extern uint8_t* CUSTOM_CHAR_MAP[];

} // namespace ui