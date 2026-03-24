#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/Container.hpp"

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
};

} // namespace components
} // namespace ui