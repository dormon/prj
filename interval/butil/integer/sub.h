#ifndef BUTIL_INTEGER_SUBTRACTION_H_INCLUDED
#define BUTIL_INTEGER_SUBTRACTION_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/intset.h>
#include <butil/integer/value.h>

#include <butil/integer/detail/sub.h>

namespace butil {
namespace integer {

template<typename X, typename Y>
constexpr auto sub(X x, Y y) noexcept
{
    using result = interval_sub<X, Y>;
    using common = typename interval_type<result, X, Y>::fast;
    return intset_value<result>(
        static_cast<common>(intset_traits<X>::value(x))
        - intset_traits<Y>::value(y));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_SUBTRACTION_H_INCLUDED
