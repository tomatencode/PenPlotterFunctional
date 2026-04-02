#pragma once

#include "../core/Widget.hpp"
#include "../core/Container.hpp"
#include <functional>
#include <vector>
#include <memory>

namespace ui {
namespace widgets {

enum SwitchEvaluationMode
{
    Eager,
    Lazy
};

struct SwitchProps
{
    std::function<bool()> selector;
    SwitchEvaluationMode evaluationMode = SwitchEvaluationMode::Eager;
};

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
        {}

        T getValue() const {
            return _value;
        }

    private:
        T _value;
    };

    template<typename... Branches>
    requires (std::same_as<std::unique_ptr<Branch>, std::decay_t<Branches>> && ...)
    Switch(SwitchProps props, Branches&&... branches)
        : _selector(props.selector),
        _branches(makeBranches(std::forward<Branches>(branches)...)),
        _evaluationMode(props.evaluationMode),
        _cacheValid(false)
    {}

    Switch(SwitchProps props, std::vector<std::unique_ptr<Branch>> branches)
        : _selector(props.selector),
        _branches(std::move(branches)),
        _evaluationMode(props.evaluationMode),
        _cacheValid(false)
    {}

    void render(Renderer& r, Rect canvasBox) override
    {
        Widget* current = getCurrentBranch();
        if (current) {
            current->render(r, canvasBox);
        }
    }

    Size measure() const override
    {
        Widget* current = getCurrentBranch();
        if (current) {
            return current->measure();
        }
        return Size{0, 0};
    }

    bool canExpandHorizontally() const override
    {
        Widget* current = getCurrentBranch();
        if (current) {
            return current->canExpandHorizontally();
        }
        return false;
    }

    bool canExpandVertically() const override
    {
        Widget* current = getCurrentBranch();
        if (current) {
            return current->canExpandVertically();
        }
        return false;
    }

    size_t getChildCount() const override
    {
        if (auto current = getCurrentBranch()) {
            return 1;
        }
        return 0;
    }

    Widget* getChild(size_t index) const override
    {
        if (index != 0) return nullptr;
        return getCurrentBranch();
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
    SwitchEvaluationMode _evaluationMode;

    // Caching
    mutable Branch* _current = nullptr;
    mutable T _currentValue = T{};
    mutable bool _cacheValid;

    Branch* getCurrentBranch() const {
        if (_evaluationMode == SwitchEvaluationMode::Lazy && _cacheValid) return _current;

        _cacheValid = true;

        T value = _selector();
        
        if (value != _currentValue) {
            _currentValue = value;
            _current = nullptr;
            
            for (auto& branch : _branches) {
                if (branch->getValue() == value) {
                    _current = branch.get();
                    break;
                }
                else {
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