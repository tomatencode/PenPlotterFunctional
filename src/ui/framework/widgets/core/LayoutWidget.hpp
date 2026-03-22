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
        // Set parent pointers for children
        for (const auto& child : _ownedChildren) {
            if (child)
                child->setParent(this);
        }
    }

    // for dynamic layouts, allow adding/removing children after construction
    virtual void addChild(std::unique_ptr<Widget> child) {
        if (child)
            child->setParent(this);
        _ownedChildren.push_back(std::move(child));
    }
    virtual void clearChildren() {
        _ownedChildren.clear();
    }
    virtual void removeChild(size_t index) {
        if (index < _ownedChildren.size()) {
            _ownedChildren.erase(_ownedChildren.begin() + index);
        }
    }

    // for traversal
    virtual size_t childCount() const override { return _ownedChildren.size(); }
    virtual Widget* child(size_t index) const override { 
        if (index < _ownedChildren.size())
            return _ownedChildren[index].get();
        return nullptr;
    }

    // for rendering and measuring, only consider enabled children
    virtual size_t EnChildCount() const {
        size_t count = 0;
        for (const auto& child : _ownedChildren) {
            if (child && child->isEnabled()) {
                ++count;
            }
        }
        return count;
    }
    virtual Widget* EnChild(size_t index) const {
        std::vector<Widget*> unHiddenChildren;
        for (const auto& child : _ownedChildren) {
            if (child && child->isEnabled()) {
                unHiddenChildren.push_back(child.get());
            }
        }

        if (index < unHiddenChildren.size())
            return unHiddenChildren[index];
        return nullptr;
    }
private:
    std::vector<std::unique_ptr<Widget>> _ownedChildren;
};

} // namespace widgets
} // namespace ui
