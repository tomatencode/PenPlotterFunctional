#pragma once

#include <memory>
#include <vector>

#include "Widget.hpp"

namespace ui {
namespace widgets {

class LayoutWidget : public Widget
{
public:
    // Owning constructor: takes ownership of the widgets
    LayoutWidget(std::vector<std::unique_ptr<Widget>>&& ownedChildren)
        : _ownedChildren(std::move(ownedChildren))
    {
    }

    virtual size_t childCount() const override { return _ownedChildren.size(); }

    virtual Widget* child(size_t index) const override {
        if (index < _ownedChildren.size())
            return _ownedChildren[index].get();
        return nullptr;
    }
private:
    std::vector<std::unique_ptr<Widget>> _ownedChildren;
};

} // namespace widgets
} // namespace ui
