#pragma once

#include<tuple>

template<typename>
struct FunctionArgumentType;

template<typename RETURN_TYPE,typename... ARGUMENTS_TYPES>
struct FunctionArgumentType<RETURN_TYPE(ARGUMENTS_TYPES...)>{
  using type = std::tuple<ARGUMENTS_TYPES...>;
};

template<typename>
struct FunctionReturnType;

template<typename RETURN_TYPE,typename... ARGUMENTS_TYPES>
struct FunctionReturnType<RETURN_TYPE(ARGUMENTS_TYPES...)>{
  using type = RETURN_TYPE;
};

template<typename>
struct MemberFunctionArgumentType;

template<typename RETURN_TYPE,typename CLASS_TYPE,typename... ARGUMENTS_TYPES>
struct MemberFunctionArgumentType<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)>{
  using type = std::tuple<ARGUMENTS_TYPES...>;
};

template<typename>
struct MemFceClassType;

template<typename RETURN_TYPE,typename CLASS_TYPE,typename...ARGUMENTS_TYPES>
struct MemFceClassType<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)const>{
  using type = CLASS_TYPE;
};

template<typename RETURN_TYPE,typename CLASS_TYPE,typename...ARGUMENTS_TYPES>
struct MemFceClassType<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)>{
  using type = CLASS_TYPE;
};

template<typename>
struct MemberFunctionReturnType;

template<typename RETURN_TYPE,typename CLASS_TYPE,typename...ARGUMENTS_TYPES>
struct MemberFunctionReturnType<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)const>{
  using type = RETURN_TYPE;
};

template<typename RETURN_TYPE,typename CLASS_TYPE,typename...ARGUMENTS_TYPES>
struct MemberFunctionReturnType<RETURN_TYPE(CLASS_TYPE::*)(ARGUMENTS_TYPES...)>{
  using type = RETURN_TYPE;
};

