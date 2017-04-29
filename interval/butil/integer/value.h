#ifndef BUTIL_INTEGER_VALUE_H_INCLUDED
#define BUTIL_INTEGER_VALUE_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/intset.h>

namespace butil {
namespace integer {

template<typename Set, typename Int = typename intset_type<Set>::least>
class intset_value
{
    static_assert(std::is_integral<Int>::value, "");
    static_assert(interval_includes<Int, Set>::value, "");
public :
    using intset_type = Set;
    using value_type = Int;

    static constexpr value_type min = intset_traits<Set>::min;
    static constexpr value_type max = intset_traits<Set>::max;

    explicit constexpr intset_value(value_type x) noexcept
        : m_value(x) {}

    value_type value() const noexcept
    {
        return m_value;
    }

    explicit operator value_type() const noexcept
    {
        return value();
    }
private :
    value_type m_value;
};

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_VALUE_H_INCLUDED
