#pragma once

#include "../core/Widget.hpp"
#include "../core/Container.hpp"
#include <functional>
#include <map>
#include <memory>

namespace ui {
namespace widgets {

template <typename T>
class Switch : public Widget
{
public:
    Switch(std::function<T()> selector, std::function<std::map<T, std::unique_ptr<Widget>>()> branchesFactory)
        : _selector(selector), _branches()
    {
        auto branchesMap = branchesFactory();
        for (auto& [key, widget] : branchesMap) {
            _branches[key] = std::make_unique<Branch>(std::move(widget));
        }
    }

    void render(Renderer& r, Rect canvasBox) override
    {
        Widget* current = getCurrentWidget();
        if (current) {
            current->render(r, canvasBox);
        }
    }

    Size measure() const override
    {
        Widget* current = getCurrentWidget();
        if (current) {
            return current->measure();
        }
        return Size{0, 0};
    }

    bool canExpandHorizontally() const override
    {
        Widget* current = getCurrentWidget();
        if (current) {
            return current->canExpandHorizontally();
        }
        return false;
    }

    bool canExpandVertically() const override
    {
        Widget* current = getCurrentWidget();
        if (current) {
            return current->canExpandVertically();
        }
        return false;
    }

    size_t childCount() const override
    {
        return _branches.size();
    }

    Widget* child(size_t index) const override
    {
        if (index >= _branches.size()) return nullptr;
        auto it = _branches.begin();
        std::advance(it, index);
        return it->second.get();
    }

private:

    class Branch : public Container
    {
    public:
        Branch(std::unique_ptr<Widget> child)
            : Container(std::move(child))
        {
        }

        void enable() {
            _enabled = true;
        }

        void disable() {
            _enabled = false;
        }

        bool isEnabled() const override {
            return _enabled;
        }
        
    private:
        bool _enabled = false;
    };

    std::function<T()> _selector;
    std::map<T, std::unique_ptr<Branch>> _branches;

    // Caching
    mutable Branch* _current = nullptr;
    mutable T _currentValue;

    Branch* getCurrentWidget() const {
        T value = _selector();
        
        if (value != _currentValue) {
            _currentValue = value;

            auto it = _branches.find(value);
            if (it != _branches.end()) {
                _current = it->second.get();
                _current->enable();
            } else {
                _current = nullptr;
            }
            // Disable all other branches
            for (auto& [key, branch] : _branches) {
                if (branch.get() != _current) {
                    branch->disable();
                }
            }
        }
        return _current;
    }
};


} // namespace widgets
} // namespace ui