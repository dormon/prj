#ifndef BUTIL_INTEGER_COMPARISON_H_INCLUDED
#define BUTIL_INTEGER_COMPARISON_H_INCLUDED

#include <butil/integer/intset.h>

#include <butil/integer/detail/cmp.h>

namespace butil {
namespace integer {
namespace cmp {

template<typename A, typename B>
constexpr std::enable_if_t<!std::is_integral<A>::value || !std::is_integral<B>::value, bool> eq(A a, B b) noexcept
{
    return !intset_less<A, B>::value && !intset_less<B, A>::value
        && eq(intset_traits<A>::value(a), intset_traits<B>::value(b));
}

template<typename A, typename B>
constexpr std::enable_if_t<!std::is_integral<A>::value || !std::is_integral<B>::value, bool> lt(A a, B b) noexcept
{
    return intset_less<A, B>::value || lt(intset_traits<A>::value(a), intset_traits<B>::value(b));
}

template<typename A, typename B>
constexpr std::enable_if_t<!std::is_integral<A>::value || !std::is_integral<B>::value, bool> le(A a, B b) noexcept
{
    return intset_less_equal<A, B>::value || le(intset_traits<A>::value(a), intset_traits<B>::value(b));
}

//
// -- Equality --
//

template<typename A, typename B>
constexpr bool ne(A a, B b) noexcept
{
    return !eq(a, b);
}

template<typename A, typename B, typename C, typename ... Args>
constexpr bool eq(A a, B b, C c, Args ... args) noexcept
{
    return eq(a, b) && eq(b, c, args...);
}

//
// -- Strict nonequality --
//

template<typename A, typename B>
constexpr bool gt(A a, B b) noexcept
{
    return lt(b, a);
}

template<typename A, typename B, typename C, typename ... Args>
constexpr bool lt(A a, B b, C c, Args ... args) noexcept
{
    return lt(a, b) && lt(b, c, args...);
}

template<typename A, typename B, typename C, typename ... Args>
constexpr bool gt(A a, B b, C c, Args ... args) noexcept
{
    return gt(a, b) && gt(b, c, args...);
}

//
// -- Nonequality --
//

template<typename A, typename B>
constexpr bool ge(A a, B b) noexcept
{
    return le(b, a);
}

template<typename A, typename B, typename C, typename ... Args>
constexpr bool le(A a, B b, C c, Args ... args) noexcept
{
    return le(a, b) && le(b, c, args...);
}

template<typename A, typename B, typename C, typename ... Args>
constexpr bool ge(A a, B b, C c, Args ... args) noexcept
{
    return ge(a, b) && ge(b, c, args...);
}

}//namespace cmp
}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_COMPARISON_H_INCLUDED
