#pragma once

#include <memory>

#include "../core/Widget.hpp"
#include "../../text/TextSource.hpp"

namespace ui {
namespace widgets {

class Label : public Widget
{
public:
    // Takes ownership of a text source.
    Label(std::unique_ptr<TextSource> textSource);

    // Convenience constructors for common sources
    Label(const char* text);

    // Render the label within the given canvas
    void render(Renderer& r, Rect canvasBox) override;

    // Return the preferred size of the label
    Size measure() const override;

private:
    std::unique_ptr<TextSource> _text;
};

} // namespace widgets
} // namespace ui
