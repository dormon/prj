#ifndef BUTIL_INTEGER_CAST_H_INCLUDED
#define BUTIL_INTEGER_CAST_H_INCLUDED

#include <butil/integer/cmp.h>
#include <butil/integer/detail/signed_cast.h>
#include <butil/integer/detail/unsigned_cast.h>

#include <cassert>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

// [un]signed_cast is imported from sign_cast.h

template<typename D,
    D Min  = boost::integer_traits<D>::const_min,
    D Max  = boost::integer_traits<D>::const_max,
    typename S>
constexpr D saturate_cast(S x) noexcept
{
    static_assert(Min <= Max, "Invalid boundaries");
    return cmp::lt(x,Min) ? Min : cmp::lt(Max,x) ? Max : static_cast<D>(x);
}

template<typename D,
    D Min  = boost::integer_traits<D>::const_min,
    D Max  = boost::integer_traits<D>::const_max,
    typename S>
constexpr D assert_cast(S x) noexcept
{
    static_assert(Min <= Max, "Invalid boundaries");
    return assert(cmp::le(Min, x, Max)), static_cast<D>(x);
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_CAST_H_INCLUDED
