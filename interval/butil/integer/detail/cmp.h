#ifndef BUTIL_INTEGER_DETAIL_CMP_H_INCLUDED
#define BUTIL_INTEGER_DETAIL_CMP_H_INCLUDED

#include <butil/integer/detail/unsigned_cast.h>

#include <type_traits>

#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

/** \brief Integer comparison that avoids dangerous casts.
 *
 * \code
 * int signed_value = -1;
 * unsigned unsigned_value = 1;
 * if(signed_value > unsigned_value) // good compilers warn here
 *     std::cout << "The world is cruel." << std::endl;
 * if(cmp::le(signed_value, unsigned_value))
 *     std::cout << "But there is hope." << std::endl;
 * \endcode
 *
 * The functions (except ne) can take unlimited number of arguments.
 * For example le(a,b,c) tests a<b<c (a<b && b<c).
 */
namespace cmp {

namespace detail {

// Compare integers
// - normally when signs are the same
// - first against zero if the signs are different
// unsigned_cast better shows the conversion

template<typename A, typename B>
constexpr bool int_eq(A a, B b) noexcept
{
    static_assert(std::is_integral<A>::value, "Type A must be integer");
    static_assert(std::is_integral<B>::value, "Type B must be integer");
#pragma GCC diagnostic ignored "-Wsign-compare"
    return std::is_signed<A>::value == std::is_signed<B>::value
        ? a == b
        : std::is_signed<A>::value
            ? (a >= 0) && (unsigned_cast(a) == b)
            : (b >= 0) && (a == unsigned_cast(b));
#pragma GCC diagnostic pop
}

template<typename A, typename B>
constexpr bool int_lt(A a, B b) noexcept
{
    static_assert(std::is_integral<A>::value, "Type A must be integer");
    static_assert(std::is_integral<B>::value, "Type B must be integer");
#pragma GCC diagnostic ignored "-Wsign-compare"
    return std::is_signed<A>::value == std::is_signed<B>::value
        ? a < b
        : std::is_signed<A>::value
            ? (a < 0) || (unsigned_cast(a) < b)
            : (b > 0) && (a < unsigned_cast(b));
#pragma GCC diagnostic pop
}

template<typename A, typename B>
constexpr bool int_le(A a, B b) noexcept
{
    static_assert(std::is_integral<A>::value, "Type A must be integer");
    static_assert(std::is_integral<B>::value, "Type B must be integer");
#pragma GCC diagnostic ignored "-Wsign-compare"
    return std::is_signed<A>::value == std::is_signed<B>::value
        ? a <= b
        : std::is_signed<A>::value
            ? (a <  0) || (unsigned_cast(a) <= b)
            : (b >= 0) && (a <= unsigned_cast(b));
#pragma GCC diagnostic pop
}

}//namespace detail

template<typename Subset, typename Superset>
struct is_subset_of
    : std::integral_constant<bool,
        detail::int_le(boost::integer_traits<Superset>::const_min, boost::integer_traits<Subset>::const_min) &&
        detail::int_le(boost::integer_traits<Subset>::const_max, boost::integer_traits<Superset>::const_max)>
{
    static_assert(std::is_integral<Subset>::value, "Subset must be integer");
    static_assert(std::is_integral<Superset>::value, "Superset must be integer");
};

namespace detail {

/** \brief Test if boundaries of A and B lie inside boundaries of their common_type.
 */
template<typename A, typename B>
struct common_type_safe_impl
{
    using common = std::common_type_t<A, B>;
    using type = std::integral_constant<bool, is_subset_of<A, common>::value && is_subset_of<B, common>::value>;
};

}//namespace detail

/** \brief Does conversion to std::common_type preserve values?
 */
template<typename A, typename B>
using common_type_safe = typename detail::common_type_safe_impl<A, B>::type;

//
// -- Comparisons --
//
// Implement int_... again with use of common_type_safe
// - works as detail::int_... but elliminates unnecessary test for less than zero (e.g. for uint and long)
// - optimizer will simplify called detail::int_...
//

template<typename A, typename B>
constexpr std::enable_if_t<std::is_integral<A>::value && std::is_integral<B>::value, bool>
    eq(A a, B b) noexcept
{
#pragma GCC diagnostic ignored "-Wsign-compare"
    return common_type_safe<A, B>::value ? a == b : detail::int_eq(a, b);
#pragma GCC diagnostic pop
}

template<typename A, typename B>
constexpr std::enable_if_t<std::is_integral<A>::value && std::is_integral<B>::value, bool>
    lt(A a, B b) noexcept
{
#pragma GCC diagnostic ignored "-Wsign-compare"
    return common_type_safe<A, B>::value ? a < b : detail::int_lt(a, b);
#pragma GCC diagnostic pop
}

template<typename A, typename B>
constexpr std::enable_if_t<std::is_integral<A>::value && std::is_integral<B>::value, bool>
    le(A a, B b) noexcept
{
#pragma GCC diagnostic ignored "-Wsign-compare"
    return common_type_safe<A, B>::value ? a <= b : detail::int_le(a, b);
#pragma GCC diagnostic pop
}

}//namespace cmp
}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_DETAIL_CMP_H_INCLUDED
