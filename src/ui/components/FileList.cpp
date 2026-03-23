#include "FileList.hpp"

#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"
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
    : Container(std::make_unique<widgets::ScrollableVerticalLayout>(
        widgets::ScrollableVerticalLayoutStyle{},
        std::vector<std::unique_ptr<widgets::Widget>>{}
    )), _onFileSelected(std::move(onFileSelected))
{
    reload();
}

void FileList::reload() {
    widgets::ScrollableVerticalLayout* layout = static_cast<widgets::ScrollableVerticalLayout*>(widgets::Container::child(0));

    if (!layout) return;

    layout->clearChildren();

    auto files = storage::fsListFiles();
    for (const auto& file : files) {
        auto label = widgets::make_widget<widgets::Label>(file.c_str());
        auto button = widgets::make_widget<widgets::Button>(
            std::move(label),
            fileButtonStyle,
            [this, file]() { _onFileSelected(file); }
        );
        layout->addChild(std::move(button));
    }
}

} // namespace components
} // namespace ui