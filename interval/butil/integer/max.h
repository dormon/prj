#ifndef BUTIL_INTEGER_MAXIMUM_H_INCLUDED
#define BUTIL_INTEGER_MAXIMUM_H_INCLUDED

#include <butil/integer/cmp.h>
#include <butil/integer/intset.h>

#include <butil/integer/detail/max.h>

namespace butil {
namespace integer {

template<typename T>
constexpr T max(T x) noexcept
{
    return x;
}

template<typename T, typename U>
constexpr auto max(T x, U y) noexcept
{
    return intset_traits<std::conditional_t<intset_greater<T, U>::value, T, U>>::make(
        cmp::gt(x, y) ? intset_traits<T>::value(x) : intset_traits<U>::value(y));
}

template<typename T, typename U, typename V, typename ... Args>
constexpr auto max(T x, U y, V z, Args ... args) noexcept
{
    return max(x, max(y, z, args...));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_MAXIMUM_H_INCLUDED
