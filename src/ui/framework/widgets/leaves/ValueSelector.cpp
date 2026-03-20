#include "ValueSelector.hpp"

namespace ui {
namespace widgets {

// Explicit template instantiations for common numeric types.
// Add other instantiations as needed.

template class ValueSelector<int>;
template class ValueSelector<float>;
template class ValueSelector<double>;

} // namespace widgets
} // namespace ui
