#pragma once

#include <cstdint>

namespace ui {

enum CustomChar
{
    CURSOR_L = 0,
    CURSOR_R = 1,
    CURSOR_DWN_L = 2,
    CURSOR_DWN_R = 3,
    BOX_CHECKD = 4,
    BOX_EMPTY = 5,
    ENTER = 6,
    PROGRESS_FULL = 7
};

extern uint8_t* CUSTOM_CHAR_MAP[];

} // namespace ui