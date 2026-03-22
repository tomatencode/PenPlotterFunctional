#include "SwitchWidget.hpp"

namespace ui {
namespace widgets {

SwitchWidget::SwitchWidget(std::function<bool()> en, std::unique_ptr<Widget>&& child)
    : ContainerWidget(std::move(child)), _enabledCallback(en)
{
}

bool SwitchWidget::isEnabled() const
{
    return _enabledCallback();
}

} // namespace widgets
} // namespace ui