#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/Container.hpp"

namespace ui {
namespace components {

class FileList : public widgets::Container
{
public:
    FileList(std::function<void(const String&)> onFileSelected);
    void reload() override;
private:
    std::function<void(const String&)> _onFileSelected;
};

} // namespace components
} // namespace ui