#ifndef BUTIL_INTEGER_DETAIL_MIN_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_MIN_H_INCLUDED

#include <type_traits>

namespace butil {
namespace integer {

template<typename T, T ... Args>
struct integral_min;

template<typename T, T A>
struct integral_min<T, A>
    : std::integral_constant<T, A>
{};

template<typename T, T A, T B>
struct integral_min<T, A, B>
    : std::integral_constant<T, (A<B) ? A : B>
{};

template<typename T, T A, T B, T C, T ... Args>
struct integral_min<T, A, B, C, Args...>
    : integral_min<T, integral_min<T, A, B>::value, C, Args...>
{};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_MIN_H_INCLUDED
