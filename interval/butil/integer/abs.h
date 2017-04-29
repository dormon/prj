#ifndef BUTIL_INTEGER_ABSOLUTE_VALUE_H_INCLUDED
#define BUTIL_INTEGER_ABSOLUTE_VALUE_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/intset.h>
#include <butil/integer/value.h>

#include <butil/integer/detail/abs.h>

namespace butil {
namespace integer {
#if 0
/** \brief Signed abs that can't collide with cmath abs.
 */
template<typename X>
constexpr X sabs(X x) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    return assert(!abs_overflow(x)),
        std::is_unsigned<X>::value ? x : std::abs(x);
}
#endif

template<typename X>
constexpr auto iabs(X x) noexcept
{
    return intset_value<interval_abs<X>>(uabs(intset_traits<X>::value(x)));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_ABSOLUTE_VALUE_H_INCLUDED
