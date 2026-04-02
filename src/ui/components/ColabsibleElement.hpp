#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/Container.hpp"
#include "../framework/widgets/layouts/LinearLayout.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/leaves/Spacer.hpp"
#include "../framework/widgets/leaves/Conditional.hpp"

#include "../framework/text/DynamicGlyphString.hpp"

#include "../framework/text/GlyphString.hpp"
#include "../framework/text/CustomChars.hpp"


namespace ui {
namespace components {

struct CollabsibleElementProps
{
    GlyphString label;
};

class CollabsibleElement : public widgets::Container
{
public:
    CollabsibleElement(
        CollabsibleElementProps props,
        std::unique_ptr<widgets::Widget> content
    )
    : Container(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Vertical},
            std::make_unique<widgets::Button>(
                widgets::ButtonProps{
                    .style = {
                        .leftNormal   = DynamicGlyphString(
                            [this]() -> GlyphString {
                            return _expanded ? "-" : "+";
                        }),
                        .rightNormal  = GlyphString{},

                        .leftFocused  = DynamicGlyphString([this]() -> GlyphString {
                            return _expanded ? Glyph(CustomChar::CollapsibleExpanded) : '>';
                        }),
                        .rightFocused = GlyphString{},

                        .leftPressed  = DynamicGlyphString(
                            [this]() -> GlyphString {
                                return _expanded ? Glyph(CustomChar::CollapsibleExpanded) : '>';
                            }),
                        .rightPressed = GlyphString{}
                    },
                    .onPress = [this]() {
                        _expanded = !_expanded;
                    }
                },
                std::make_unique<widgets::Label>(props.label)
            ),
            
            std::make_unique<widgets::Conditional>(
                widgets::ConditionalProps{
                    .condition = [this]() { return _expanded; }
                },
                std::make_unique<widgets::LinearLayout>(
                    widgets::LinearLayoutStyle{.axis = widgets::Axis::Horizontal},
                    std::make_unique<widgets::Spacer>(1,0), // indent
                    std::move(content)
                )
            )
        )
    )
    {}
private:
    bool _expanded = false;
};

} // namespace components
} // namespace ui