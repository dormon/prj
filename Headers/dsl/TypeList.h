#pragma once

#include<tuple>

namespace dsl{

  template<typename...TYPES>
    struct TypeList{

      using types = std::tuple<TYPES...>;

      template<size_t ID>
        using type = typename std::tuple_element<ID,types>::type;

      size_t static const nofTypes = std::tuple_size<types>::value;
    };

}
