#ifndef BUTIL_INTEGER_DETAIL_MAX_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_MAX_H_INCLUDED

#include <type_traits>

namespace butil {
namespace integer {

template<typename T, T ... Args>
struct integral_max;

template<typename T, T A>
struct integral_max<T, A>
    : std::integral_constant<T, A>
{};

template<typename T, T A, T B>
struct integral_max<T, A, B>
    : std::integral_constant<T, (A<B) ? B : A>
{};

template<typename T, T A, T B, T C, T ... Args>
struct integral_max<T, A, B, C, Args...>
    : integral_max<T, integral_max<T, A, B>::value, C, Args...>
{};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_MAX_H_INCLUDED
