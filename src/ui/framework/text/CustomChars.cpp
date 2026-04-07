#include "CustomChars.hpp"

namespace ui {

// Each byte represents a row (5 LSBs used)
uint8_t WifiSymbol_BITMAP[8] = {
	0b00000,
	0b01110,
	0b10001,
	0b00000,
	0b01110,
	0b00000,
	0b00100,
	0b00000
};

uint8_t NoWifiSymbol_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00100,
	0b00100,
	0b00100,
	0b00000,
	0b00100,
	0b00000
};

uint8_t TextEditorEnter_BITMAP[8] = {
	0b00000,
	0b00000,
	0b10000,
	0b10000,
	0b10010,
	0b11111,
	0b00010,
	0b00000
};

uint8_t TextEditorDelete_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b01000,
	0b11111,
	0b01000,
	0b00000,
	0b00000
};

uint8_t DegSymbol_BITMAP[8] = {
	0b00111,
	0b00101,
	0b00111,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

uint8_t CustomChar_6_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

uint8_t CustomChar_7_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

uint8_t Ellipsis_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b10101
};


uint8_t* CUSTOM_CHAR_MAP[] = {
    WifiSymbol_BITMAP,
    NoWifiSymbol_BITMAP,
    TextEditorEnter_BITMAP,
    TextEditorDelete_BITMAP,
    DegSymbol_BITMAP,
    CustomChar_6_BITMAP,
    CustomChar_7_BITMAP,
    Ellipsis_BITMAP
};

} // namespace ui