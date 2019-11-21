#include <iostream>
#include <utility>
using namespace std;

template < size_t Size >
constexpr auto make_costring(char const (&) [Size]);

template < size_t Size >
struct costring
{
    using data_type = char const (&) [Size+1];

    constexpr size_t size() const
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

    template < size_t I >
    constexpr costring<Size+I-1> append(char const (&cstr)[I]) const
    {
        return append( std::make_index_sequence<Size>()
                     , std::make_index_sequence<I-1>()
                     , cstr );
    }

    template < size_t I >
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
    char arr[Size+1];

    template < size_t I, size_t ... Indices >
    constexpr costring(char const (&str) [I], std::index_sequence<Indices...>)
        : arr{str[Indices]..., '\0'}
    {}

    template < size_t ... Indices >
    constexpr costring<Size+1> push_back(char c, std::index_sequence<Indices...>) const
    {
        char const newArr[] = {arr[Indices]..., c, '\0'};
        return costring<Size+1>(newArr);
    }

    template < size_t ... Indices >
    constexpr costring<Size+1> push_front(char c, std::index_sequence<Indices...>) const
    {
        char const newArr[] = {c, arr[Indices]..., '\0'};
        return costring<Size+1>(newArr);
    }

    template < size_t ... ThisIndices
             , size_t ... ThatIndices
             , size_t I >
    constexpr costring<Size+I-1> append( std::index_sequence<ThisIndices...>
                                     , std::index_sequence<ThatIndices...>
                                     , char const (&cstr) [I] ) const
    {
        char const newArr[] = {arr[ThisIndices]..., cstr[ThatIndices]..., '\0'};
        return costring<Size+I-1>(newArr);
    }
};

template < size_t Size >
constexpr auto make_costring(char const (&cstr) [Size])
{
    return costring<Size-1>(cstr);
}

template < size_t I >
constexpr costring<I + 1> operator + (costring<I> const& s, char c)
{
    return s.push_back(c);
}

template < size_t I >
constexpr costring<I + 1> operator + (char c, costring<I> const& s)
{
    return s.push_front(c);
}

template < size_t I0, size_t I1>
constexpr costring<I0+I1> operator + (costring<I0> const& s0, costring<I1> const& s1)
{
    return s0.append(s1);
}

template<size_t...I,typename std::enable_if_t<sizeof...(I)==0,int> = 0>
constexpr size_t sum(){return 0;}

template<size_t I,size_t...Is>
constexpr size_t sum(){return I-1+sum<Is...>();}


template<size_t I>
using CST = char const (&)[I];

template<size_t...Is,typename std::enable_if_t<sizeof...(Is)==0,int> = 0>
constexpr costring<0> getName(CST<Is>...a){
  return make_costring("");
}


template<size_t I0,size_t...Is>
constexpr costring<sum<I0,Is...>()> getName(CST<I0> a,CST<Is>... b){
  return make_costring(a) + getName(b...);
}

template<size_t...Is>
constexpr CST<sum<Is...>()+1> concat(CST<Is>...a){
  return getName(a...).c_str();
}

int main()
{
  //std::cerr << getName("dormon").c_str() << std::endl;
  //std::cerr << getName("dormon","jede").c_str() << std::endl;
  //std::cerr << getName("dormon","jede","na").c_str() << std::endl;
  auto const v = getName("ab").c_str();//concat("ab"); 
  std::cerr << "#" <<(int)v[0] <<"#" << std::endl;
  //std::cerr << concat("dormon","jede","na","kole","veze") << std::endl;
  //std::cerr << concat("dormon","jede","na","kole","veze","sebou") << std::endl;
  //std::cerr << concat("dormon","jede","na","kole","veze","sebou","kobzole") << std::endl;
  //std::cerr << concat("dormon",".","vars","par") << std::endl;
  //std::cerr << concat("dormon",".","vars","par") << std::endl;

  return 0;
}
