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

uint8_t CollapsibleExpanded_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b10001,
	0b01010,
	0b00100,
	0b00000,
	0b00000
};

uint8_t CustomChar_4_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};

uint8_t CustomChar_5_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00000,
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

uint8_t CustomChar_8_BITMAP[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000,
	0b00000
};


uint8_t* CUSTOM_CHAR_MAP[] = {
    WifiSymbol_BITMAP,
    NoWifiSymbol_BITMAP,
    CollapsibleExpanded_BITMAP,
    CustomChar_4_BITMAP,
    CustomChar_5_BITMAP,
    CustomChar_6_BITMAP,
    CustomChar_7_BITMAP,
    CustomChar_8_BITMAP
};

} // namespace ui