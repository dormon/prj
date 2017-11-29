#pragma once

#include<dsl/TypeList.h>

namespace dsl{

  template<typename>
    struct FunctionTraits;

  template<typename RETURN_TYPE,typename... ARGUMENTS_TYPES>
    struct FunctionTraits<RETURN_TYPE(ARGUMENTS_TYPES...)>:TypeList<ARGUMENTS_TYPES...>{
      using returnType = RETURN_TYPE;
    };


  //convert member function to regular function
  template<typename RETURN_TYPE,typename CLASS_TYPE,typename... ARGUMENTS_TYPES>
    struct FunctionTraits<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)>:TypeList<CLASS_TYPE*,ARGUMENTS_TYPES...>{
      using returnType = RETURN_TYPE;
    };

  //convert const member function to regular function
  template<typename RETURN_TYPE,typename CLASS_TYPE,typename... ARGUMENTS_TYPES>
    struct FunctionTraits<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)const>:TypeList<CLASS_TYPE const*,ARGUMENTS_TYPES...>{
      using returnType = RETURN_TYPE;
    };



  template<typename>
    struct MemberFunctionTraits;

  template<typename RETURN_TYPE,typename CLASS_TYPE,typename... ARGUMENTS_TYPES>
    struct MemberFunctionTraits<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)>:TypeList<ARGUMENTS_TYPES...>{
      using returnType = RETURN_TYPE;
      using classType  = CLASS_TYPE ;
    };

  template<typename RETURN_TYPE,typename CLASS_TYPE,typename... ARGUMENTS_TYPES>
    struct MemberFunctionTraits<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)const>:TypeList<ARGUMENTS_TYPES...>{
      using returnType = RETURN_TYPE;
      using classType  = CLASS_TYPE ;
    };

  //convert regular function to member function
  template<typename RETURN_TYPE,typename CLASS_TYPE,typename... ARGUMENTS_TYPES>
    struct MemberFunctionTraits<RETURN_TYPE(CLASS_TYPE,ARGUMENTS_TYPES...)const>:TypeList<ARGUMENTS_TYPES...>{
      using returnType = RETURN_TYPE;
      using classType  = CLASS_TYPE ;
    };

}
