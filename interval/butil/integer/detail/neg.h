#ifndef BUTIL_INTEGER_DETAIL_NEG_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_NEG_H_INCLUDED

#include <type_traits>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

template<typename X>
constexpr bool neg_negative_overflow(X a) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    return std::is_unsigned<X>::value && (a > 0);
}

template<typename X>
constexpr bool neg_positive_overflow(X a) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    return std::is_signed<X>::value && (a == boost::integer_traits<X>::const_min);
}

template<typename X>
constexpr bool neg_overflow(X a) noexcept
{
    return neg_negative_overflow<X>(a)
        || neg_positive_overflow<X>(a);
}

namespace detail {

template<bool Overflow, typename X, X A>
struct integral_neg_base
    : std::integral_constant<X, -A>
{
};

template<typename X, X A>
struct integral_neg_base<true, X, A>
{
    using value_type = X;
};

}//namespace detail

template<typename X, X A>
struct integral_neg
    : detail::integral_neg_base<neg_overflow<X>(A), X, A>
{
    static constexpr bool negative_overflow = neg_negative_overflow<X>(A);
    static constexpr bool positive_overflow = neg_positive_overflow<X>(A);
    static constexpr bool overflow = negative_overflow | positive_overflow;
};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_NEG_H_INCLUDED
