#pragma once

#include <functional>
#include <memory>

#include "ui/framework/widgets/core/Container.hpp"
#include "ui/framework/widgets/leaves/Label.hpp"
#include "ui/framework/widgets/leaves/Spacer.hpp"
#include "ui/framework/widgets/leaves/ValueSelector.hpp"
#include "ui/framework/widgets/layouts/LinearLayout.hpp"

namespace ui {
namespace components {

template <typename ValueType>
struct LabeledValueSelectorProps
{
    std::string labelText;
    widgets::ValueSelectorProps<ValueType> valueSelectorProps;
};

template <typename ValueType>
class LabeledValueSelector : public widgets::Container
{
public:
    LabeledValueSelector(LabeledValueSelectorProps<ValueType> props)
    : Container(
        std::make_unique<widgets::LinearLayout>(
            widgets::LinearLayoutStyle{.axis = widgets::Axis::Horizontal},
            std::make_unique<widgets::Label>(std::move(props.labelText)),
            std::make_unique<widgets::Spacer>(1, 1),
            std::make_unique<widgets::ValueSelector<ValueType>>(std::move(props.valueSelectorProps))
        )
    )
    {}
};

} // namespace components
} // namespace ui