#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include <variant>
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

    // operator+ preserves dynamic type if the provider holds a DynamicGlyphString
    GlyphStringProvider operator+(const GlyphString& other) const
    {
        if (std::holds_alternative<DynamicGlyphString>(_glyphs))
            return GlyphStringProvider(std::get<DynamicGlyphString>(_glyphs) + other);
        return GlyphStringProvider(std::get<GlyphString>(_glyphs) + other);
    }

    GlyphStringProvider operator+(Glyph glyph) const
    {
        if (std::holds_alternative<DynamicGlyphString>(_glyphs))
            return GlyphStringProvider(std::get<DynamicGlyphString>(_glyphs) + glyph);
        return GlyphStringProvider(std::get<GlyphString>(_glyphs) + glyph);
    }

    GlyphStringProvider operator+(const GlyphStringProvider& other) const
    {
        bool thisDynamic  = std::holds_alternative<DynamicGlyphString>(_glyphs);
        bool otherDynamic = std::holds_alternative<DynamicGlyphString>(other._glyphs);

        if (thisDynamic || otherDynamic)
        {
            auto lhs = thisDynamic
                ? std::get<DynamicGlyphString>(_glyphs)
                : DynamicGlyphString([s = std::get<GlyphString>(_glyphs)]() { return s; });
            auto rhs = otherDynamic
                ? std::get<DynamicGlyphString>(other._glyphs)
                : DynamicGlyphString([s = std::get<GlyphString>(other._glyphs)]() { return s; });
            return GlyphStringProvider(lhs + rhs);
        }
        return GlyphStringProvider(std::get<GlyphString>(_glyphs) + std::get<GlyphString>(other._glyphs));
    }

    GlyphStringProvider& operator+=(const GlyphString& other)
    {
        if (std::holds_alternative<DynamicGlyphString>(_glyphs))
            std::get<DynamicGlyphString>(_glyphs) += other;
        else
            _glyphs = std::get<GlyphString>(_glyphs) + other;
        return *this;
    }

    GlyphStringProvider& operator+=(Glyph glyph)
    {
        if (std::holds_alternative<DynamicGlyphString>(_glyphs))
            std::get<DynamicGlyphString>(_glyphs) += glyph;
        else
            _glyphs = std::get<GlyphString>(_glyphs) + glyph;
        return *this;
    }

    GlyphStringProvider& operator+=(const GlyphStringProvider& other)
    {
        *this = *this + other;
        return *this;
    }

private:
    std::variant<GlyphString, DynamicGlyphString> _glyphs;
};

} // namespace ui