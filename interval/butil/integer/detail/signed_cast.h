#ifndef BUTIL_INTEGER_DETAIL_SIGNED_CAST_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_SIGNED_CAST_H_INCLUDED

#include <cassert>

#include <type_traits>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

/** \brief Convert value to a corresponding signed type.
 */
template<typename S>
constexpr std::make_signed_t<S> signed_cast(S x) noexcept
{
    return assert(x <= boost::integer_traits<std::make_signed_t<S>>::const_max), x;
}

constexpr bool signed_cast(bool x) noexcept
{
    return x;
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_SIGNED_CAST_H_INCLUDED
