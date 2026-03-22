#include "FileList.hpp"

#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"
#include "../framework/widgets/leaves/ButtonWidget.hpp"
#include "../framework/widgets/leaves/LabelWidget.hpp"
#include "../framework/widgets/Builder.hpp"

#include "../../storage/FileSystem.hpp"

#include <vector>
#include <memory>


namespace ui {
namespace components {

widgets::ButtonStyle fileButtonStyle = {
    .leftNormal = GLYPH_NONE,
    .rightNormal = GLYPH_NONE,
    .leftFocused = '>',
    .rightFocused = GLYPH_NONE,
    .leftPressed = '-',
    .rightPressed = GLYPH_NONE
};


FileList::FileList(std::function<void(const String&)> onFileSelected)
    : ContainerWidget(std::make_unique<widgets::ScrollableVerticalLayout>(
        widgets::ScrollableVerticalLayoutStyle{},
        std::vector<std::unique_ptr<widgets::Widget>>{}
    )), _onFileSelected(std::move(onFileSelected))
{
    widgets::ScrollableVerticalLayout* layout = static_cast<widgets::ScrollableVerticalLayout*>(widgets::ContainerWidget::child(0));
    auto files = storage::fsListFiles();
    for (const auto& file : files) {
        auto label = widgets::make_widget<widgets::LabelWidget>(file.c_str());
        auto button = widgets::make_widget<widgets::ButtonWidget>(
            std::move(label),
            fileButtonStyle,
            [this, file]() { _onFileSelected(file); }
        );
        layout->addChild(std::move(button));
    }
}

} // namespace components
} // namespace ui