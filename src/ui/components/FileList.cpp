#include "FileList.hpp"

#include "../framework/widgets/leaves/Button.hpp"
#include "../framework/widgets/leaves/Spacer.hpp"

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
    _fileManager.registerFileObserver(this);
    
    auto layout = std::make_unique<widgets::ScrollableVerticalLayout>(
        widgets::ScrollableVerticalLayoutStyle{}
    );

    _layout = layout.get();
    setChild(std::move(layout));

    reload();
}

FileList::~FileList() {
    _fileManager.unregisterFileObserver(this);
}

void FileList::reload() {
    if (!_layout) return;

    _layout->clearChildren();

    auto files = _fileManager.listFiles(PLOTTING_DIRECTORY + "/");
    for (const auto& file : files) {
        auto button = std::make_unique<widgets::Button>(
            widgets::ButtonProps{
                .style = ui::styles::listButtonStyle,
                .onPress = [this, file]() { _onFileSelected(file); }
            },
            std::make_unique<widgets::Label>(file.substring(0, file.length() - 6))
        );
        _layout->addChild(std::move(button));
    }
}

void FileList::onFileEvent(FileEvent event, const String& path) {
    if (event == FileEvent::ADDED || event == FileEvent::REMOVED) {
        reload();
    }
}

} // namespace components
} // namespace ui