#ifndef BUTIL_INTEGER_INTSET_H_INCLUDED
#define BUTIL_INTEGER_INTSET_H_INCLUDED

#include <butil/integer/select.h>

#include <butil/integer/detail/cmp.h>

#include <type_traits>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {
namespace detail {

template<bool IsIntegral, typename T>
struct intset_traits_base
{
    static_assert(cmp::le(T::min, T::max), "");
    // TODO : deal with different types
    static_assert(std::is_same<decltype(T::min), decltype(T::max)>::value, "");
    // boundaries
    using value_type = std::decay_t<decltype(T::min)>;
    static constexpr value_type min = T::min;
    static constexpr value_type max = T::max;

    static constexpr value_type value(T x) noexcept
    {
        return static_cast<value_type>(x);
    }

    static constexpr T make(value_type x) noexcept
    {
        return static_cast<T>(x);
    }
};

template<typename T>
struct intset_traits_base<true, T>
{
    static_assert(std::is_integral<T>::value, "");
    using value_type = T;
    static constexpr value_type min = boost::integer_traits<T>::const_min;
    static constexpr value_type max = boost::integer_traits<T>::const_max;

    static constexpr T value(T x) noexcept
    {
        return x;
    }

    static constexpr T make(T x) noexcept
    {
        return x;
    }
};

template<typename T, T C>
struct intset_traits_base<false, std::integral_constant<T,C>>
{
    using value_type = T;
    static constexpr value_type min = C;
    static constexpr value_type max = C;

    static constexpr T value(std::integral_constant<T,C>) noexcept
    {
        return C;
    }

    static constexpr std::integral_constant<T,C> make(T) noexcept
    {
        return std::integral_constant<T,C>();
    }
};

}//namespace detail

/** \brief Traits of an set of integers.
 *
 * value_type - boundary type + storage type if applicable
 * boundaries min and max
 *
 * Works for anything with static constant members min and max.
 * Specialized for
 * - builtin integers
 * - std::integral_constant
 */
template<typename T>
struct intset_traits
    : detail::intset_traits_base<std::is_integral<T>::value, T>
{};

//------------------------------------------------------------------------------
// -- Relation operators --
//
// ordering - A < B <=> forall a in A, b in B : a < b
//------------------------------------------------------------------------------

template<typename A, typename B>
using intset_less = std::integral_constant<bool,
      cmp::lt(intset_traits<A>::max, intset_traits<B>::min)>;

template<typename A, typename B>
using intset_greater = intset_less<B, A>;

template<typename A, typename B>
using intset_less_equal = std::integral_constant<bool,
      cmp::le(intset_traits<A>::max, intset_traits<B>::min)>;

template<typename A, typename B>
using intset_greater_equal = intset_less_equal<B, A>;

//------------------------------------------------------------------------------
// -- Type selection --
//------------------------------------------------------------------------------

enum prefer_type
{
    prefer_unsigned = 0,
    prefer_signed = 1,
};

namespace detail {

template<bool want_signed, typename T>
struct intset_type_impl
    : int_range_value_t<intset_traits<T>::min, intset_traits<T>::max>
{};

template<typename T>
struct intset_type_impl<false, T>
    : boost::uint_value_t<intset_traits<T>::max>
{};

}//namespace detail

template<typename T, prefer_type Pref = prefer_signed>
struct intset_type
{
    static constexpr bool want_signed = (intset_traits<T>::min < 0)
        || ((Pref == prefer_signed) && (intset_traits<T>::max <= INTMAX_MAX));
    using least = typename detail::intset_type_impl<want_signed, T>::least;
    using fast = typename boost::int_fast_t<least>::type;
    using max = int_max_t<least>;
};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_INTSET_H_INCLUDED
