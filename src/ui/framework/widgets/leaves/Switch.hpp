#pragma once

#include "../core/Widget.hpp"
#include "../core/Container.hpp"
#include <functional>
#include <vector>
#include <memory>

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
            : Container(std::move(child)),
            _value(value)
        {
        }

        void enable() {
            _enabled = true;
        }

        void disable() {
            _enabled = false;
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
        _lazy(lazy),
        _cacheValid(false)
    {}

    Switch(std::function<T()> selector, bool lazy, std::vector<std::unique_ptr<Branch>> branches)
        : _selector(selector),
        _branches(std::move(branches)),
        _lazy(lazy),
        _cacheValid(false)
    {}

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

    size_t getChildCount() const override
    {
        if (auto current = getCurrentWidget()) {
            return 1;
        }
        return 0;
    }

    Widget* getChild(size_t index) const override
    {
        if (index != 0) return nullptr;
        return getCurrentWidget();
    }

    void reload() override {
        for (auto& branch : _branches) {
            if (branch)
                branch->reload();
        }

        _cacheValid = false;
    }

private:
    std::function<T()> _selector;
    std::vector<std::unique_ptr<Branch>> _branches;
    bool _lazy;

    // Caching
    mutable Branch* _current = nullptr;
    mutable T _currentValue = T{};
    mutable bool _cacheValid;

    Branch* getCurrentWidget() const {
        if (_lazy && _cacheValid) return _current;

        _cacheValid = true;

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