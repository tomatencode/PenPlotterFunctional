#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/Container.hpp"
#include "../framework/widgets/leaves/Label.hpp"
#include "../framework/widgets/leaves/Spacer.hpp"
#include "../framework/widgets/leaves/TextEditor.hpp"
#include "../framework/widgets/layouts/LinearLayout.hpp"

namespace ui {
namespace components {

template <typename ValueType>
struct LabeledTextEditorProps
{
    std::string labelText;
    widgets::TextEditorProps textEditorProps;
};

template <typename ValueType>
class LabeledTextEditor : public widgets::Container
{
public:
    LabeledTextEditor(LabeledTextEditorProps<ValueType> props)
    : Container(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Horizontal},
            std::make_unique<widgets::Label>(std::move(props.labelText)),
            std::make_unique<widgets::Spacer>(1, 1),
            std::make_unique<widgets::TextEditor>(std::move(props.textEditorProps))
        )
    )
    {}
};

} // namespace components
} // namespace ui