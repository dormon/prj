#ifndef BUTIL_INTEGER_NEGATION_H_INCLUDED
#define BUTIL_INTEGER_NEGATION_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/intset.h>
#include <butil/integer/value.h>

#include <butil/integer/detail/neg.h>

namespace butil {
namespace integer {

template<typename X>
constexpr auto neg(X x) noexcept
{
    using result = interval_neg<X>;
    using common = typename interval_type<X, result>::fast;
    return intset_value<result>(-static_cast<common>(intset_traits<X>::value(x)));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_NEGATION_H_INCLUDED
