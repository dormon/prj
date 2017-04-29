#ifndef BUTIL_INTEGER_SELECT_H_INCLUDED
#define BUTIL_INTEGER_SELECT_H_INCLUDED

#include <type_traits>

#include <boost/integer.hpp>
#include <boost/integer_traits.hpp>

namespace butil {
namespace integer {

/** \brief Select signed integer that can hold all values in range.
 */
template<intmax_t Min, intmax_t Max>
struct int_range_value_t
{
    static_assert(Min <= Max, "Invalid boundaries"); 

    using least = std::common_type_t<
        typename boost::int_min_value_t<Min>::least,
        typename boost::int_max_value_t<Max>::least>;

    static_assert(boost::integer_traits<least>::const_min <= Min, "");
    static_assert(boost::integer_traits<least>::const_max >= Max, "");

    using fast = typename boost::int_fast_t<least>::fast;

    static_assert(boost::integer_traits<fast>::const_min <= Min, "");
    static_assert(boost::integer_traits<fast>::const_max >= Max, "");
};

/** \brief Get [u]intmax_t with signedness of T.
 */
template<typename T>
struct int_max
    : std::conditional<std::is_signed<T>::value, intmax_t, uintmax_t>
{
    static_assert(std::is_integral<T>::value, "");
};

template<typename T>
using int_max_t = typename int_max<T>::type;

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_SELECT_H_INCLUDED
