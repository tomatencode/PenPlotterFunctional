#pragma once

#include "../core/Widget.hpp"
#include <functional>
#include <map>
#include <memory>

namespace ui {
namespace widgets {

template <typename T>
class Switch : public Widget
{
public:
    Switch(std::function<T()> selector, bool lazy, std::function<std::map<T, std::unique_ptr<Widget>>()> branchesFactory)
        : _selector(selector), _lazy(lazy), _branches(branchesFactory()), _cached(false)
    {
        if (_lazy) {
            _cachedValue = _selector();
            _cached = true;
        }
    }

    void render(Renderer& r, Rect canvasBox) override
    {
        if (!_cached) {
            _cachedValue = _selector();
            _cached = true;
        }
        auto it = _branches.find(_cachedValue);
        if (it != _branches.end()) {
            it->second->render(r, canvasBox);
        }
    }

    Size measure() const override
    {
        if (!_cached) {
            _cachedValue = _selector();
            _cached = true;
        }
        auto it = _branches.find(_cachedValue);
        if (it != _branches.end()) {
            return it->second->measure();
        }
        return Size{0, 0};
    }

    bool canExpandHorizontally() const override
    {
        if (!_cached) {
            _cachedValue = _selector();
            _cached = true;
        }
        auto it = _branches.find(_cachedValue);
        if (it != _branches.end()) {
            return it->second->canExpandHorizontally();
        }
        return false;
    }

    bool canExpandVertically() const override
    {
        if (!_cached) {
            _cachedValue = _selector();
            _cached = true;
        }
        auto it = _branches.find(_cachedValue);
        if (it != _branches.end()) {
            return it->second->canExpandVertically();
        }
        return false;
    }

    bool isEnabled() const override
    {
        if (!_cached) {
            _cachedValue = _selector();
            _cached = true;
        }
        auto it = _branches.find(_cachedValue);
        if (it != _branches.end()) {
            return it->second->isEnabled();
        }
        return true;
    }

    size_t childCount() const override
    {
        if (!_cached) {
            _cachedValue = _selector();
            _cached = true;
        }
        auto it = _branches.find(_cachedValue);
        if (it != _branches.end()) {
            return it->second->childCount();
        }
        return 0;
    }

    Widget* child(size_t index) const override
    {
        if (!_cached) {
            _cachedValue = _selector();
            _cached = true;
        }
        auto it = _branches.find(_cachedValue);
        if (it != _branches.end()) {
            return it->second->child(index);
        }
        return nullptr;
    }

private:
    std::function<T()> _selector;
    bool _lazy;
    std::map<T, std::unique_ptr<Widget>> _branches;
    mutable T _cachedValue;
    mutable bool _cached;
};

} // namespace widgets
} // namespace ui