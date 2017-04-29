#ifndef BUTIL_INTEGER_ADDITION_H_INCLUDED
#define BUTIL_INTEGER_ADDITION_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/intset.h>
#include <butil/integer/value.h>

#include <butil/integer/detail/add.h>
/*
#include <cassert>

#include <type_traits>

#include <boost/integer_traits.hpp>
*/
namespace butil {
namespace integer {

template<typename X>
constexpr X add(X a) noexcept
{
    return a;
}

template<typename X, typename ... Args>
constexpr auto add(X x, Args ... args) noexcept
{
    using rhs = decltype(add(args...));
    using result = interval_add<X, rhs>;
    using common = typename interval_type<result, rhs>::fast;
    return intset_value<result>(
        static_cast<common>(intset_traits<X>::value(x)) +
        intset_traits<rhs>::value(add(args...)));
}

/*
template<typename X>
constexpr X saturate_add(X a, X b) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    return add_negative_overflow(a, b)
        ? boost::integer_traits<X>::const_min
        : add_positive_overflow(a, b)
            ? boost::integer_traits<X>::const_max
            : a+b;
}
*/
}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_ADDITION_H_INCLUDED
