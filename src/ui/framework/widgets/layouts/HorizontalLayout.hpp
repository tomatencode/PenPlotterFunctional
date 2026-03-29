#pragma once

#include "LinearLayout.hpp"

namespace ui {
namespace widgets {

class HorizontalLayout : public LinearLayout
{
public:
    HorizontalLayout(const LinearLayoutStyle& style,
                     std::vector<std::unique_ptr<Widget>>&& children)
        : LinearLayout(Axis::Horizontal, style, std::move(children))
    {}

    template <typename... Children>
    HorizontalLayout(const LinearLayoutStyle& style, Children&&... children)
        : LinearLayout(Axis::Horizontal, style, std::forward<Children>(children)...)
    {}
};

} // namespace widgets
} // namespace ui