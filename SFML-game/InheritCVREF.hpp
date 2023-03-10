
#include <concepts>

template <typename T, typename U>
using inherit_const_t = std::conditional_t<std::is_const_v<T>, std::add_const_t<U>, U>;

template <typename T, typename U>
using inherit_volatile_t = std::conditional_t<std::is_volatile_v<T>, std::add_volatile_t<U>, U>;

template <typename T, typename U>
using inherit_cv_qualifiers_t = inherit_const_t<T, inherit_volatile_t<T, U>>;

template <typename T, typename U>
using inherit_lvalue_qualifier_t = std::conditional_t<std::is_lvalue_reference_v<T>, std::add_lvalue_reference_t<U>, U>;

template <typename T, typename U>
using inherit_rvalue_qualifier_t = std::conditional_t<std::is_rvalue_reference_v<T>, std::add_rvalue_reference_t<U>, U>;

template <typename T, typename U>
using inherit_ref_qualifiers_t = inherit_rvalue_qualifier_t<T, inherit_lvalue_qualifier_t<T, U>>;

template <typename T, typename U>
using inherit_cvref_qualifiers_t = inherit_ref_qualifiers_t<T, inherit_cv_qualifiers_t<std::remove_reference_t<T>, U>>;

template <typename T, typename U>
  requires std::is_reference_v<T>
inline auto&& inherit_qualifiers(U& value)
{
  return std::forward<inherit_cvref_qualifiers_t<T, U>>(value);
}
