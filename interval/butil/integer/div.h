#ifndef BUTIL_INTEGER_DIVISION_H_INCLUDED
#define BUTIL_INTEGER_DIVISION_H_INCLUDED

#include <cassert>

#include <type_traits>
#include <utility>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

template<typename X>
constexpr bool div_positive_overflow(X a, X b) noexcept
{
    static_assert(std::is_integral<X>::value, "");
    // for signed : min / -1 -> -min (max+1)
    return std::is_signed<X>::value && (a == boost::integer_traits<X>::const_min) && (b == -1);
}

template<typename X>
constexpr bool div_overflow(X a, X b) noexcept
{
    return div_positive_overflow<X>(a, b);
}

enum class mod_sign
{
    positive = 0x0,
    dividend = 0x1,
    divisor  = 0x2
};

namespace detail {

template<bool Signed, mod_sign S>
struct div_mod_impl;

template<mod_sign S>
struct div_mod_impl<false, S>
{
    template<typename X>
    static constexpr std::pair<X,X> divmod(X n, X d, mod_sign) noexcept
    {
        static_assert(std::is_integral<X>::value, "");
        static_assert(std::is_unsigned<X>::value, "");
        return std::make_pair(n/d, n%d);
    }
};

template<>
struct div_mod_impl<true, mod_sign::dividend>
{
    template<typename X>
    static constexpr std::pair<X,X> divmod(X n, X d, mod_sign s) noexcept
    {
        static_assert(std::is_integral<X>::value, "");
        static_assert(std::is_signed<X>::value, "");
        return
            s == mod_sign::positive
                ? ((n%d == 0) || (n >= 0)
                    ? std::make_pair(n/d,n%d)
                    : d > 0
                        ? std::make_pair(n/d-1,n%d+d)
                        : std::make_pair(n/d+1,n%d-d)
                  )// positive end
                : s == mod_sign::dividend
                    ? std::make_pair(n/d,n%d)
                    : // s == mod_sign::divisor
                        (n%d == 0) || ((n >= 0) == (d > 0))
                            ? std::make_pair(n/d,n%d)
                            : std::make_pair(n/d-1,n%d+d)
            ;
    }
};

constexpr mod_sign DEFAULT_MOD_SIGN = static_cast<mod_sign>(
        static_cast<int>(-3%2 < 0) | (static_cast<int>(3%-2 < 0)<<1)
    );

}//namespace detail

template<typename X>
constexpr std::pair<X,X> divmod(X n, X d, mod_sign s) noexcept
{
    return assert((d != 0) && !div_overflow(n, d)), 
        detail::div_mod_impl<std::is_signed<X>::value, detail::DEFAULT_MOD_SIGN>::divmod(n,d,s);
}

template<typename X>
constexpr X div(X n, X d, mod_sign s) noexcept
{
    return divmod(n,d,s).first;
}

template<typename X>
constexpr X mod(X n, X d, mod_sign s) noexcept
{
    return divmod(n,d,s).second;
}

namespace detail {

template<bool Overflow, typename X, X A, X B, mod_sign S>
struct integral_div_base
    : std::integral_constant<X, div(A, B, S)>
{
};

template<typename X, X A, X B, mod_sign S>
struct integral_div_base<true, X, A, B, S>
{
    using value_type = X;
};

}//namespace detail

template<typename X, X A, X B, mod_sign S>
struct integral_div
    : detail::integral_div_base<div_overflow<X>(A, B), X, A, B, S>
{
    static constexpr bool negative_overflow = false;
    static constexpr bool positive_overflow = div_positive_overflow<X>(A, B);
    static constexpr bool overflow = positive_overflow;
};

template<typename X, X A, X B, mod_sign S>
struct integral_mod
    : std::integral_constant<X, mod(A, B, S)>
{
    static constexpr bool negative_overflow = false;
    static constexpr bool positive_overflow = false;
    static constexpr bool overflow = false;
};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DIVISION_H_INCLUDED
