#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/Container.hpp"
#include "../framework/widgets/layouts/ScrollableVerticalLayout.hpp"

#include "storage/FileManager.hpp"

namespace ui {
namespace components {

class FileList : public widgets::Container
{
public:
    FileList(FileManager& fileManager, std::function<void(const String&)> onFileSelected);
    void reload();
private:
    FileManager& _fileManager;
    std::function<void(const String&)> _onFileSelected;
    widgets::ScrollableVerticalLayout* _layout;
};

} // namespace components
} // namespace ui