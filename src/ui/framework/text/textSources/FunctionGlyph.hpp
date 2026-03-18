#pragma once

#include "../TextSource.hpp"

namespace ui {

class FunctionGlyph : public TextSource
{
public:
    using TextFunc = const Glyph* (*)();

    FunctionGlyph(TextFunc func) : _func(func) {}

    const Glyph* getGlyphs() const override
    {
        return _func();
    }

private:
    TextFunc _func;
};

} // namespace ui