#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/Widget.hpp"
#include "../framework/widgets/core/Size.hpp"
#include "../framework/widgets/layouts/HorizontalLayout.hpp"
#include "../framework/widgets/core/LayoutStyle.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"

#include "WifiIndicator.hpp"

class HeaderLine: public ui::widgets::Widget
{
public:
    HeaderLine(const char* headerText, bool backButton,
               std::function<void()> onBackPress = nullptr);
    ~HeaderLine();

    void render(ui::Renderer& r, ui::widgets::Rect canvasBox) override;

    ui::widgets::Size measure() const override;

    // Allow focus/navigation systems to traverse the back button (if present)
    size_t childCount() const override;
    ui::widgets::Widget* child(size_t index) const override;
private:
    // Layout used to evenly space the elements; owns its children
    std::unique_ptr<ui::widgets::Widget> _layout;
};