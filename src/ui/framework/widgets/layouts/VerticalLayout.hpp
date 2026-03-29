#pragma once

#include "LinearLayout.hpp"

namespace ui {
namespace widgets {

class VerticalLayout : public LinearLayout
{
public:
    VerticalLayout(const LinearLayoutStyle& style,
                   std::vector<std::unique_ptr<Widget>>&& children)
        : LinearLayout(Axis::Vertical, style, std::move(children))
    {}

    template <typename... Children>
    VerticalLayout(const LinearLayoutStyle& style, Children&&... children)
        : LinearLayout(Axis::Vertical, style, std::forward<Children>(children)...)
    {}
};

} // namespace widgets
} // namespace ui