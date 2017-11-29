#pragma once

#include<dsl/IsTypeContainedInRestOfTypes.h>

namespace dsl{

  constexpr bool isEveryTypeUnique(){
    return true;
  }

  template<typename HEAD,typename...TAIL>
  constexpr bool isEveryTypeUnique(){
    return !isFirstTypeContainedInRestOfTypes<HEAD,TAIL...>() &&
      isEveryTypeUnique<TAIL...>();
  }

}
