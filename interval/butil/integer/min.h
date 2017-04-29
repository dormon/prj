#ifndef BUTIL_INTEGER_MINIMUM_H_INCLUDED
#define BUTIL_INTEGER_MINIMUM_H_INCLUDED

#include <butil/integer/cmp.h>
#include <butil/integer/intset.h>

#include <butil/integer/detail/min.h>

namespace butil {
namespace integer {

template<typename T>
constexpr T min(T x) noexcept
{
    return x;
}

template<typename T, typename U>
constexpr auto min(T x, U y) noexcept
{
    return intset_traits<std::conditional_t<intset_less<T, U>::value, T, U>>::make(
        cmp::lt(x, y) ? intset_traits<T>::value(x) : intset_traits<U>::value(y));
}

template<typename T, typename U, typename V, typename ... Args>
constexpr auto min(T x, U y, V z, Args ... args) noexcept
{
    return min(x, min(y, z, args...));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_MINIMUM_H_INCLUDED
