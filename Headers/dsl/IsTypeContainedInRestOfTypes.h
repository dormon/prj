#pragma once

#include<type_traits>

namespace dsl{

  constexpr bool isFirstTypeContainedInRestOfTypes(){
    return false;
  }

  template<typename FIRST_TYPE>
  constexpr bool isFirstTypeContainedInRestOfTypes(){
    return false;
  }

  template<typename FIRST_TYPE,typename HEAD_OF_REST,typename TAIL_OF_REST>
  constexpr bool isFirstTypeContainedInRestOfTypes(){
    return std::is_same<FIRST_TYPE,HEAD_OF_REST>::value ||
      isFirstTypeContainedInRestOfTypes<FIRST_TYPE,TAIL_OF_REST>();
  }

}
