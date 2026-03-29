#include "FileList.hpp"

#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"
#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Label.hpp"

#include "../styles/ButtonStyles.hpp"

#include "config/job_config.hpp"

#include <vector>
#include <memory>


namespace ui {
namespace components {

FileList::FileList(FileManager& fileManager, std::function<void(const String&)> onFileSelected)
    : Container(std::make_unique<widgets::ScrollableVerticalLayout>(
        widgets::ScrollableVerticalLayoutStyle{},
        std::vector<std::unique_ptr<widgets::Widget>>{}
    )),
    _fileManager(fileManager),
    _onFileSelected(std::move(onFileSelected))
{
    reload();
}

void FileList::reload() {
    widgets::ScrollableVerticalLayout* layout = static_cast<widgets::ScrollableVerticalLayout*>(widgets::Container::child(0));

    if (!layout) return;

    layout->clearChildren();

    auto files = _fileManager.listFiles(PLOTTING_DIRECTORY + "/");
    for (const auto& file : files) {
        auto button = std::make_unique<widgets::Button>(
            file,
            ui::styles::listButtonStyle,
            [this, file]() { _onFileSelected(file); }
        );
        layout->addChild(std::move(button));
    }
}

} // namespace components
} // namespace ui