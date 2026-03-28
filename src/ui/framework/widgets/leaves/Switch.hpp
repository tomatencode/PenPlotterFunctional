#pragma once

#include "../core/Widget.hpp"
#include "../core/Container.hpp"
#include <functional>
#include <vector>
#include <memory>
#include <concepts>

namespace ui {
namespace widgets {

template <typename T>
class Switch : public Widget
{
public:

    class Branch : public Container
    {
    public:
        Branch(T value, std::unique_ptr<Widget> child)
            : Container(std::move(child)), _value(value)
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

        T getValue() const {
            return _value;
        }
    private:
        bool _enabled = false;
        T _value;
    };

    template<typename... Branches>
    requires (std::same_as<std::unique_ptr<Branch>, std::decay_t<Branches>> && ...)
    Switch(std::function<T()> selector, bool lazy, Branches&&... branches)
        : _selector(selector),
        _branches(makeBranches(std::forward<Branches>(branches)...)),
        _lazy(lazy)
    {
        for (auto& branch : _branches) {
            branch->setParent(this);
        }
    }

    Switch(std::function<T()> selector, bool lazy, std::vector<std::unique_ptr<Branch>> branches)
        : _selector(selector), _branches(branches), _lazy(lazy)
    {
        for (auto& branch : _branches) {
            branch->setParent(this);
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
        
        return _branches[index].get();
    }

    void reload() override {
        for (auto& branch : _branches) {
            if (branch)
                branch->reload();
        }
        // Clear cache to force re-evaluation of selector on next render/measure
        _current = nullptr;
    }

private:
    std::function<T()> _selector;
    std::vector<std::unique_ptr<Branch>> _branches;
    bool _lazy;

    // Caching
    mutable Branch* _current = nullptr;
    mutable T _currentValue;

    Branch* getCurrentWidget() const {
        if (_lazy && _current) return _current;

        T value = _selector();
        
        if (value != _currentValue) {
            _currentValue = value;

            for (auto& branch : _branches) {
                if (branch->getValue() == value) {
                    branch->enable();
                    _current = branch.get();
                    break;
                }
                else {
                    branch->disable();
                }
            }
        }
        return _current;
    }

    template<typename... Branches>
    std::vector<std::unique_ptr<Branch>> makeBranches(Branches&&... branches)
    {
        std::vector<std::unique_ptr<Branch>> v;
        v.reserve(sizeof...(branches));
        (v.push_back(std::forward<Branches>(branches)), ...);
        return v;
    }
};


} // namespace widgets
} // namespace ui