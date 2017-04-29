#ifndef BUTIL_INTEGER_DETAIL_ABS_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_ABS_H_INCLUDED

#include <type_traits>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

template<typename X>
constexpr bool abs_positive_overflow(X x) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    return std::is_signed<X>::value
        && (x == boost::integer_traits<X>::const_min);
}

template<typename X>
constexpr bool abs_overflow(X x) noexcept
{
    return abs_positive_overflow<X>(x);
}

/** \brief Unsigned absolute value that can't overflow.
 */
template<typename X>
constexpr std::make_unsigned_t<X> uabs(X x) noexcept
{
    using uint = std::make_unsigned_t<X>;
    static_assert(std::is_unsigned<X>::value
        || (boost::integer_traits<X>::const_min == -boost::integer_traits<X>::const_max-1), "");
    return std::is_unsigned<X>::value ? x :
        abs_overflow(x)
            ? (uint(1)+boost::integer_traits<X>::const_max)//casts X to unsigned type
            : x < 0 ? -x : x;// can't use std::abs in constexpr functions yet
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_ABS_H_INCLUDED
