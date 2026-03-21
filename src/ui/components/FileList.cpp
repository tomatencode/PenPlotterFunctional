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
    : ContainerWidget( [onFileSelected]() -> std::unique_ptr<widgets::Widget> {

        auto children = std::vector<std::unique_ptr<widgets::Widget>>{};
        auto files = storage::fsListFiles();

        for (const auto& file : files) {
            auto label = widgets::make_widget<widgets::LabelWidget>(file.c_str());
            auto button = widgets::make_widget<widgets::ButtonWidget>(
                std::move(label),
                fileButtonStyle,
                [onFileSelected, file]() { onFileSelected(file); }
            );
            children.push_back(std::move(button));
        }
        
        return std::make_unique<widgets::ScrollableVerticalLayout>(
            widgets::ScrollableVerticalLayoutStyle{},
            std::move(children)
        );
    }() )
{
}

} // namespace components
} // namespace ui