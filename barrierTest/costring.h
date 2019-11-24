#pragma once

#include <utility>

template<std::size_t I>
using CST = char const (&)[I];

template < std::size_t Size >
constexpr auto make_costring(char const (&) [Size]);

template < std::size_t Size >
struct costring
{
    using data_type = CST<Size+1>;

    constexpr std::size_t size() const
    {
        return Size;
    }

    constexpr data_type c_str() const
    {
        return arr;
    }

    constexpr costring<Size+1> push_back(char c) const
    {
        return push_back(c, std::make_index_sequence<Size>());
    }

    constexpr costring<Size+1> push_front(char c) const
    {
        return push_front(c, std::make_index_sequence<Size>());
    }

    template < std::size_t I >
    constexpr costring<Size+I-1> append(char const (&cstr)[I]) const
    {
        return append( std::make_index_sequence<Size>()
                     , std::make_index_sequence<I-1>()
                     , cstr );
    }

    template < std::size_t I >
    constexpr costring<Size+I> append(costring<I> const& s) const
    {
        return append( std::make_index_sequence<Size>()
                     , std::make_index_sequence<I>()
                     , s.c_str() );
    }

    constexpr costring(char const (&str) [Size+1])
        : costring(str, std::make_index_sequence<Size>())
    {}

    constexpr costring() : arr{} {}

private:
    char const arr[Size+1];

    template < std::size_t I, std::size_t ... Indices >
    constexpr costring(char const (&str) [I], std::index_sequence<Indices...>)
        : arr{str[Indices]..., '\0'}
    {}

    template < std::size_t ... Indices >
    constexpr costring<Size+1> push_back(char c, std::index_sequence<Indices...>) const
    {
        char const newArr[] = {arr[Indices]..., c, '\0'};
        return costring<Size+1>(newArr);
    }

    template < std::size_t ... Indices >
    constexpr costring<Size+1> push_front(char c, std::index_sequence<Indices...>) const
    {
        char const newArr[] = {c, arr[Indices]..., '\0'};
        return costring<Size+1>(newArr);
    }

    template < std::size_t ... ThisIndices
             , std::size_t ... ThatIndices
             , std::size_t I >
    constexpr costring<Size+I-1> append( std::index_sequence<ThisIndices...>
                                     , std::index_sequence<ThatIndices...>
                                     , char const (&cstr) [I] ) const
    {
        char const newArr[] = {arr[ThisIndices]..., cstr[ThatIndices]..., '\0'};
        return costring<Size+I-1>(newArr);
    }
};

template < std::size_t Size >
constexpr auto make_costring(char const (&cstr) [Size])
{
    return costring<Size-1>(cstr);
}

template < std::size_t I >
constexpr costring<I + 1> operator + (costring<I> const& s, char c)
{
    return s.push_back(c);
}

template < std::size_t I >
constexpr costring<I + 1> operator + (char c, costring<I> const& s)
{
    return s.push_front(c);
}

template < std::size_t I0, std::size_t I1>
constexpr costring<I0+I1> operator + (costring<I0> const& s0, costring<I1> const& s1)
{
    return s0.append(s1);
}

template<std::size_t...I,typename std::enable_if_t<sizeof...(I)==0,int> = 0>
constexpr std::size_t sum(){return 0;}

template<std::size_t I,std::size_t...Is>
constexpr std::size_t sum(){return I-1+sum<Is...>();}


template<std::size_t...Is,typename std::enable_if_t<sizeof...(Is)==0,int> = 0>
constexpr costring<0> getName(CST<Is>...a){
  return make_costring("");
}


template<std::size_t I0,std::size_t...Is>
constexpr costring<sum<I0,Is...>()> getName(CST<I0> a,CST<Is>... b){
  return make_costring(a) + getName(b...);
}

template<std::size_t...Is>
constexpr CST<sum<Is...>()+1> concat(CST<Is>...a){
  return getName(a...).c_str();
}
