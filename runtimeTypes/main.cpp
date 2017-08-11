#include<iostream>
#include<sstream>
#include<functional>
#include<map>
#include<vector>
#include<typeinfo>
#include<tuple>
#include<limits>

#include"macros.h"

template<typename...TYPES>
struct TypeList{
  using types = std::tuple<TYPES...>;
  template<size_t ID>
    using type = typename std::tuple_element<ID,types>::type;
  static const size_t nofTypes = std::tuple_size<types>::value;
  
  template<typename     ,size_t ID = 0,typename std::enable_if_t<(ID>=nofTypes),char> = 0>
    constexpr static size_t toId(){
      return ID;
    }

  template<typename TYPE,size_t ID = 0,typename std::enable_if_t<(ID< nofTypes),char> = 0>
    constexpr static size_t toId(){
      if(std::is_same<TYPE,type<ID>>::value)return ID;
      return toId<TYPE,ID+1>();
    }

  template<typename TYPE>
    constexpr static bool contains(){
      return toId<TYPE>()<nofTypes;
    }
};

using CppAtomicTypes = TypeList<
  void              ,
  bool              ,
  char              ,
  signed char       ,
  unsigned char     ,
  short             ,
  unsigned short    ,
  int               ,
  unsigned int      ,
  long              ,
  unsigned long     ,
  long long         ,
  unsigned long long,
  float             ,
  double            ,
  long double       ,
  wchar_t           ,
  char16_t          ,
  char32_t          >;

using TypeId = size_t;

class Type{
  public:
    enum class TypeType{
      ATOMIC   ,
      REFERENCE,
      CONST    ,
      POINTER  ,
      ARRAY    ,
    };
    Type(TypeType type,TypeId id):id(id),type(type){}
    virtual ~Type(){}
    TypeType getType()const{return type;}
    TypeId   getId  ()const{return id  ;}
  protected:
    TypeId   id  ;
    TypeType type;
};

#define ENABLE_IF_CPP_ATOMIC(type)\
  typename std::enable_if_t<CppAtomicTypes::contains<T>(),char>

#define ENABLE_IF_REFERENCE(type)\
  typename std::enable_if_t<std::is_reference<T>::value,char>

#define ENABLE_IF_CONST(type)\
  typename std::enable_if_t<std::is_const<T>::value,char>

#define ENABLE_IF_POINTER(type)\
  typename std::enable_if_t<std::is_pointer<T>::value && !std::is_reference<T>::value && !std::is_const<T>::value,char>

#define ENABLE_IF_ARRAY(type)\
  typename std::enable_if_t<std::is_array<T>::value && !std::is_const<T>::value,char>

class TypeRegister{
  public:
    TypeRegister(){
    }
    template<typename T>
      TypeId add(){
        auto const existingId = hasType<T>();
        if(existingId != notAType)return existingId;
        return addType<T>();
      }
    TypeId static const notAType;
  protected:
    template<typename T,ENABLE_IF_CPP_ATOMIC(T) = 0>
      TypeId addType();
    template<typename T,ENABLE_IF_CPP_ATOMIC(T) = 0>
      TypeId hasType();

#define DEFINE_ADD_OR_HAS(FCE,REMOVE,TYPE)\
  template<typename T,ENABLE_IF_##TYPE (T) = 0>\
    TypeId FCE##Type(){\
      using InnerType = typename std::remove_##REMOVE<T>::type;\
      return FCE##ReferenceOrConstOrPointerType<InnerType,Type::TypeType::TYPE>();\
    }

    DEFINE_ADD_OR_HAS(add,reference,REFERENCE)//;
    DEFINE_ADD_OR_HAS(has,reference,REFERENCE)//;
    DEFINE_ADD_OR_HAS(add,const    ,CONST    )//;
    DEFINE_ADD_OR_HAS(has,const    ,CONST    )//;
    DEFINE_ADD_OR_HAS(add,pointer  ,POINTER  )//;
    DEFINE_ADD_OR_HAS(has,pointer  ,POINTER  )//;
    template<typename T,ENABLE_IF_ARRAY(T) = 0>
      TypeId addType();
    template<typename T,ENABLE_IF_ARRAY(T) = 0>
      TypeId hasType();

    template<typename INNER_TYPE,Type::TypeType type>
      TypeId addReferenceOrConstOrPointerType();
    template<typename INNER_TYPE,Type::TypeType type>
      TypeId hasReferenceOrConstOrPointerType();

    std::map<TypeId,Type*>types;
};

TypeId const TypeRegister::notAType = std::numeric_limits<TypeId>::max();

class AtomicType: public Type{
  public:
    AtomicType(TypeId id,size_t atomicId):Type(TypeType::ATOMIC,id),atomicId(atomicId){}
    size_t getAtomicId()const{return atomicId;}
  protected:
    size_t atomicId;
};

template<Type::TypeType type>
class ReferenceOrConstOrPointerType: public Type{
  public:
    ReferenceOrConstOrPointerType(
        TypeId   const&id   ,
        TypeId   const&inner):Type(type,id),innerType(inner){}
    TypeId getInnerType()const{return innerType;}
  protected:
    TypeId innerType;
};

class ArrayType: public Type{
  public:
    ArrayType(
        TypeId const&id   ,
        TypeId const&inner,
        size_t const&size ):Type(TypeType::ARRAY,id),innerType(inner),size(size){}
    TypeId getInnerType()const{return innerType;}
    size_t getSize     ()const{return size     ;}
  protected:
    TypeId innerType;
    size_t size     ;
};

class StructType: public Type{
  public:
    StructType();
};

template<typename T,ENABLE_IF_CPP_ATOMIC(T)>
TypeId TypeRegister::addType(){
  auto const id = types.size();
  types[id] = new AtomicType(id,CppAtomicTypes::toId<T>());
  return id;
}

template<typename T,ENABLE_IF_CPP_ATOMIC(T)>
TypeId TypeRegister::hasType(){
  for(auto const&x:types)
    if(x.second->getType() == Type::TypeType::ATOMIC){
      auto const existingType = dynamic_cast<AtomicType*>(x.second);
      if(existingType->getAtomicId() != CppAtomicTypes::toId<T>())continue;
      return existingType->getId();
    }
  return notAType;
}

template<typename INNER_TYPE,Type::TypeType type>
TypeId TypeRegister::addReferenceOrConstOrPointerType(){
  auto const innerTypeId = add<INNER_TYPE>();
  auto const id = types.size();
  types[id] = new ReferenceOrConstOrPointerType<type>(id,innerTypeId);
  return id;
}

template<typename INNER_TYPE,Type::TypeType type>
TypeId TypeRegister::hasReferenceOrConstOrPointerType(){
  auto const innerTypeId = hasType<INNER_TYPE>();
  if(innerTypeId == notAType)return notAType;
  for(auto const&x:types)
    if(x.second->getType() == type){
      auto const existingType = dynamic_cast<ReferenceOrConstOrPointerType<type>*>(x.second);
      if(existingType->getInnerType() != innerTypeId)continue;
      return existingType->getId();
    }
  return notAType;
}


template<typename T,ENABLE_IF_ARRAY(T)>
TypeId TypeRegister::addType(){
  using InnerType = typename std::remove_extent_t<T>;
  auto const id = types.size();
  auto const innerType = add<InnerType>();
  auto const size = std::extent<T>::value;
  
  types[id] = new ArrayType(CppAtomicTypes::toId<T>(),innerType,size);
  return id;
}

template<typename T,ENABLE_IF_ARRAY(T)>
TypeId TypeRegister::hasType(){
  using InnerType = typename std::remove_extent_t<T>;
  auto const innerTypeId = hasType<InnerType>();
  auto const size = std::extent<T>::value;
  for(auto const&x:types)
    if(x.second->getType() == Type::TypeType::ARRAY){
      auto const existingType = dynamic_cast<ArrayType*>(x.second);
      if(existingType->getInnerType() != innerTypeId)continue;
      if(existingType->getSize     () != size       )continue;
      return existingType->getId();
    }
  return notAType;
}

int main(){
  std::cout << CppAtomicTypes::toId<signed   char>() << std::endl;
  std::cout << CppAtomicTypes::toId<         char>() << std::endl;
  std::cout << CppAtomicTypes::toId<unsigned char>() << std::endl;
  std::cout << CppAtomicTypes::toId<float        >() << std::endl;
  using TypeList = TypeList<void,int,float,double>;
  std::cout << typeid(TypeList::type<0>).name() << std::endl;

  std::cout << TypeList::nofTypes << std::endl;
#define PRINT_NESS(ness,type)\
  std::cout << "std::" << #ness << "<" << #type << "> - " << std::ness<type>::value << std::endl

  PRINT_NESS(is_const    ,int            );
  PRINT_NESS(is_const    ,int&           );
  PRINT_NESS(is_const    ,int*           );
  PRINT_NESS(is_const    ,int const      );
  PRINT_NESS(is_const    ,int const&     );
  PRINT_NESS(is_const    ,int const&     );
  PRINT_NESS(is_const    ,int const*     );
  PRINT_NESS(is_const    ,int const*const);
  PRINT_NESS(is_const    ,const int      );
  PRINT_NESS(is_const    ,const int*     );
  PRINT_NESS(is_reference,int            );
  PRINT_NESS(is_reference,int&           );
  PRINT_NESS(is_reference,int*           );
  PRINT_NESS(is_reference,int const      );
  PRINT_NESS(is_reference,int const&     );
  PRINT_NESS(is_reference,int const&     );
  PRINT_NESS(is_reference,int const*     );
  PRINT_NESS(is_reference,int const*const);
  PRINT_NESS(is_reference,const int      );
  PRINT_NESS(is_reference,const int*     );

  TypeRegister tr;
  tr.add<int            >();
  tr.add<int            &>();
  tr.add<int       const >();
  tr.add<int       const&>();
  tr.add<int      *      >();
  tr.add<int const*      >();
  tr.add<int      *const >();
  tr.add<int const*const >();

  tr.add<int      *     &>();
  tr.add<int const*     &>();
  tr.add<int      *const&>();
  tr.add<int const*const&>();

  tr.add<int[2]>();
  tr.add<int[2][2]>();
  tr.add<int const*[2]>();
  tr.add<int const*const[2]>();
  tr.add<int*[2]>();
  return 0;
}
