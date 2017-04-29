#ifndef BUTIL_INTEGER_DETAIL_ADD_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_ADD_H_INCLUDED

#include <type_traits>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

template<typename X>
constexpr bool add_negative_overflow(X a, X b) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    // a + b < min
    return (b < 0) && (a < (boost::integer_traits<X>::const_min - b));
}

template<typename X>
constexpr bool add_positive_overflow(X a, X b) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    // a + b > max
    return (b > 0) && (a > (boost::integer_traits<X>::const_max - b));
}

template<typename X>
constexpr bool add_overflow(X a, X b) noexcept
{
    return add_negative_overflow<X>(a, b)
        || add_positive_overflow<X>(a, b);
}

namespace detail {

template<bool Overflow, typename X, X A, X B>
struct integral_add_base
    : std::integral_constant<X, A+B>
{
};

template<typename X, X A, X B>
struct integral_add_base<true, X, A, B>
{
    using value_type = X;
};

}//namespace detail

template<typename X, X A, X B>
struct integral_add
    : detail::integral_add_base<add_overflow<X>(A, B), X, A, B>
{
    static constexpr bool negative_overflow = add_negative_overflow<X>(A, B);
    static constexpr bool positive_overflow = add_positive_overflow<X>(A, B);
    static constexpr bool overflow = negative_overflow | positive_overflow;
};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_ADD_H_INCLUDED
