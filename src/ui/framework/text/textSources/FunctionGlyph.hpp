#pragma once

#include <functional>

#include "../TextSource.hpp"

namespace ui {

class FunctionGlyph : public TextSource
{
public:
    using TextFunc = std::function<const Glyph*()>;

    FunctionGlyph(TextFunc func) : _func(std::move(func)) {}

    const Glyph* getGlyphs() const override
    {
        return _func();
    }

private:
    TextFunc _func;
};

} // namespace ui