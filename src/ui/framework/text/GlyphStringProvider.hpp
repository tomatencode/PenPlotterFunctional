#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include <concepts>
#include "GlyphString.hpp"
#include "DynamicGlyphString.hpp"

namespace ui {

class GlyphStringProvider {
public:
    GlyphStringProvider() {}

    GlyphStringProvider(GlyphString glyphs)
        : _glyphs(std::move(glyphs))
    {}

    GlyphStringProvider(DynamicGlyphString glyphs)
        : _glyphs(std::move(glyphs))
    {}

    // Accept anything GlyphString can be constructed from (const char*, String, Glyph, etc.)
    template<typename T>
    requires std::constructible_from<GlyphString, T>
          && (!std::same_as<std::decay_t<T>, GlyphString>)
          && (!std::same_as<std::decay_t<T>, DynamicGlyphString>)
    GlyphStringProvider(T&& text)
        : _glyphs(GlyphString(std::forward<T>(text)))
    {}

    // Accept any callable whose return type GlyphString can be constructed from
    template<typename Fn>
    requires std::invocable<Fn>
        && std::constructible_from<GlyphString, std::invoke_result_t<Fn>>
        && (!std::constructible_from<GlyphString, Fn>)  // exclude text literals
    GlyphStringProvider(Fn&& fn)
        : _glyphs(DynamicGlyphString(std::function<GlyphString()>([f = std::forward<Fn>(fn)]() -> GlyphString {
            return GlyphString(f());
        })))
    {}

    GlyphString getGlyphs() const {
        if (std::holds_alternative<GlyphString>(_glyphs)) {
            return std::get<GlyphString>(_glyphs);
        } else {
            return std::get<DynamicGlyphString>(_glyphs).getGlyphs();
        }
    }

    size_t size() const { return getGlyphs().size(); }

    Glyph operator[](size_t i) const { return i < size() ? getGlyphs()[i] : Glyph{}; }

private:
    std::variant<GlyphString, DynamicGlyphString> _glyphs;
};

} // namespace ui