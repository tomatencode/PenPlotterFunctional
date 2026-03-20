#pragma once

#include <memory>

#include "../framework/widgets/core/Widget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"

namespace ui {
namespace components {

class WifiIndicator: public ui::widgets::Widget
{
public:
    WifiIndicator();

    void render(ui::Renderer& r, ui::widgets::Rect canvasBox) override;

    ui::widgets::Size measure() const override;

private:
    std::unique_ptr<ui::widgets::LabelWidget> _label;
};

} // namespace components
} // namespace ui