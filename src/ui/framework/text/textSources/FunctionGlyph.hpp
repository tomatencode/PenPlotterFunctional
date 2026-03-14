#ifndef FUNCTION_GLYPH_HPP
#define FUNCTION_GLYPH_HPP

#include "../TextSource.hpp"

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

#endif