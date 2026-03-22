#pragma once
#include "../core/ContainerWidget.hpp"

namespace ui {
namespace widgets {

class SwitchWidget : public ContainerWidget
{
public:
    SwitchWidget(std::function<bool()> en, std::unique_ptr<Widget>&& child);

    bool isEnabled() const override;
private:
    std::function<bool()> _enabledCallback;
};

} // namespace widgets
} // namespace ui