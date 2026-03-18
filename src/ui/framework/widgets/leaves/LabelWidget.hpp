#pragma once

#include <memory>

#include "../core/Widget.hpp"
#include "../../text/TextSource.hpp"

namespace ui {
namespace widgets {

class LabelWidget : public Widget
{
public:
    // Takes ownership of a text source.
    LabelWidget(std::unique_ptr<TextSource> textSource);

    // Convenience constructors for common sources
    LabelWidget(const char* text);

    // Render the label within the given canvas
    void render(Renderer& r, Rect canvasBox) override;

    // Return the preferred size of the label
    Size measure() const override;

private:
    std::unique_ptr<TextSource> _text;
};

} // namespace widgets
} // namespace ui
