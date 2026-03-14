#pragma once

enum class HorizontalAlignment {
    Left,
    Center,
    Right
};

enum class VerticalAlignment {
    Top,
    Middle,
    Bottom
};

struct Alignment
{
    HorizontalAlignment horizontal;
    VerticalAlignment vertical;

    constexpr Alignment(HorizontalAlignment h, VerticalAlignment v) : horizontal(h), vertical(v) {}
    constexpr Alignment() : horizontal(HorizontalAlignment::Left), vertical(VerticalAlignment::Top) {}
};
