#include "FileList.hpp"

#include "../framework/widgets/leaves/Button.hpp"

#include "../styles/ButtonStyles.hpp"

#include "config/job_config.hpp"

#include <vector>
#include <memory>


namespace ui {
namespace components {

FileList::FileList(FileManager& fileManager,
                   std::function<void(const String&)> onFileSelected)
    : Container(),
      _fileManager(fileManager),
      _onFileSelected(std::move(onFileSelected))
{
    auto layout = std::make_unique<widgets::ScrollableVerticalLayout>(
        widgets::ScrollableVerticalLayoutStyle{},
        std::vector<std::unique_ptr<widgets::Widget>>{}
    );

    _layout = layout.get();
    setChild(std::move(layout));

    reload();
}

void FileList::reload() {
    if (!_layout) return;

    _layout->clearChildren();

    auto files = _fileManager.listFiles(PLOTTING_DIRECTORY + "/");
    for (const auto& file : files) {
        auto button = std::make_unique<widgets::Button>(
            file,
            ui::styles::listButtonStyle,
            [this, file]() { _onFileSelected(file); }
        );
        _layout->addChild(std::move(button));
    }
}

} // namespace components
} // namespace ui