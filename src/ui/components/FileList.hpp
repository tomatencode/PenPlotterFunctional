#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/Container.hpp"
#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"

#include "storage/FileManager.hpp"

namespace ui {
namespace components {

class FileList : public widgets::Container, public FileObserver
{
public:
    FileList(FileManager& fileManager, std::function<void(const std::string&)> onFileSelected);
    ~FileList();

    void reload();
    
private:
    FileManager& _fileManager;
    std::function<void(const std::string&)> _onFileSelected;
    widgets::ScrollableVerticalLayout* _layout;

    void onFileEvent(FileEvent event, const std::string& path) override;
};

} // namespace components
} // namespace ui