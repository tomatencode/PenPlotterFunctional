#pragma once

#include <functional>
#include <memory>

#include "../framework/widgets/core/ContainerWidget.hpp"

namespace ui {
namespace components {

class FileList: public ui::widgets::ContainerWidget
{
public:
    FileList(std::function<void(const String&)> onFileSelected);
};

} // namespace components
} // namespace ui