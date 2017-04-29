#ifndef BUTIL_INTEGER_DETAIL_UNSIGNED_CAST_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_UNSIGNED_CAST_H_INCLUDED

#include <cassert>

#include <type_traits>

namespace butil {
namespace integer {

/** \brief Convert value to a corresponding unsigned type.
 */
template<typename S>
constexpr std::make_unsigned_t<S> unsigned_cast(S x) noexcept
{
    return assert(x >= 0), x;
}

constexpr bool unsigned_cast(bool x) noexcept
{
    return x;
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_UNSIGNED_CAST_H_INCLUDED
