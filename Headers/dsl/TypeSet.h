#pragma once

#include<dsl/TypeList.h>
#include<dsl/IsEveryTypeUnique.h>

namespace dsl{

  template<typename...TYPES>//,typename std::enable_if_t<isEveryTypeUnique<TYPES...>(),char> = 0>
    struct TypeSet: TypeList<TYPES...>{

      template<typename TYPE>
        constexpr static size_t idOf(){
          constexpr auto id = idOfNoError<TYPE>();
          static_assert(id < TypeSet::nofTypes,"TypeSet::idOf() - TypeSet does not contain selected type!");
          return id;
        }

      template<typename TYPE>
        constexpr static bool contains(){
          return idOf<TYPE>()< TypeSet::nofTypes;
        }

      template<typename     ,size_t ID = 0,typename std::enable_if_t<(ID>=TypeSet::nofTypes),char> = 0>
        constexpr static size_t idOfNoError(){
          return ID;
        }

      template<typename TYPE,size_t ID = 0,typename std::enable_if_t<(ID< TypeSet::nofTypes),char> = 0>
        constexpr static size_t idOfNoError(){
          using TypeOfId = typename TypeSet::template type<ID>;
          if(std::is_same<TYPE,TypeOfId>::value)return ID;
          return idOfNoError<TYPE,ID+1>();
        }

    };

}
