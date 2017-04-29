#pragma once

#include<vector>
#include<map>
#include<set>
#include<iostream>
#include<memory>

namespace ge{
  namespace de{
    class Resource;
    class TypeRegister: public std::enable_shared_from_this<TypeRegister>{
      protected:
        class TypeDescription;
        using TypeVector = std::vector<TypeDescription*>;
      public:
        using Destructor         = void(*)(uint8_t*);
        using Constructor        = void(*)(int8_t*);
        using TypeId             = TypeVector::size_type;
        using DescriptionElement = size_t;
        using DescriptionVector  = std::vector<DescriptionElement>;
        enum TypeType{
          UNREGISTERED = 0,
          ATOMIC       = 1,
          ARRAY        = 2,
          STRUCT       = 3,
          FCE          = 4,
          TYPEID       = 5,
        };
        TypeRegister();
        virtual ~TypeRegister();
        TypeId addAtomicType(
              std::string const&name                 ,
              size_t      const&size                 ,
              Destructor  const&destructor  = nullptr,
              Constructor const&constructor = nullptr);
        TypeId addCompositeType(
            std::string       const&name       ,
            DescriptionVector const&description);
        size_t getNofTypes()const;
        std::string type2Str(size_t typeIndex)const;
        TypeType                    getTypeIdType         (TypeId id)const;
        size_t                      getNofStructElements  (TypeId id)const;
        TypeId                      getStructElementTypeId(TypeId id,size_t index)const;
        size_t                      getArraySize          (TypeId id)const;
        TypeId                      getArrayElementTypeId (TypeId id)const;
        TypeId                      getFceReturnTypeId    (TypeId id)const;
        size_t                      getNofFceArgs         (TypeId id)const;
        TypeId                      getFceArgTypeId       (TypeId id,size_t index)const;
        TypeId                      getTypeId             (std::string const&name)const;
        std::string const&          getTypeIdName         (TypeId id)const;
        std::set<std::string>const& getTypeIdSynonyms     (TypeId id)const;
        bool                        hasSynonyms           (TypeId id)const;
        bool                        areSynonyms           (std::string const&name0,std::string const&name1)const;
        size_t                      computeTypeIdSize     (TypeId id)const;
        void*alloc(TypeId id)const;
        void free(void*ptr)const;
        void*construct(TypeId id)const;
        void destroy(void*ptr,TypeId id)const;
        std::shared_ptr<Resource>sharedResource(TypeId id)const;
        std::shared_ptr<Resource>sharedResource(std::string const&name)const;
        std::shared_ptr<Resource>sharedEmptyResource(TypeId id)const;
        std::shared_ptr<Resource>sharedEmptyResource(std::string const&name)const;
        void addDestructor(TypeId id,Destructor const&destructor = nullptr);
        void addConstructor(TypeId id,Constructor const&constructor = nullptr);
      protected:
        class ArrayDescription;
        class StructDescription;
        class FunctionDescription;
        class AtomicDescription;
        TypeVector _types;
        std::map<TypeId,std::set<std::string>>_typeId2Synonyms;
        std::map<std::string,TypeId>_name2TypeId;
        TypeId _vectorIndex2TypeId(TypeId const&index)const;
        TypeId _typeId2VectorIndex(TypeId const&id   )const;
        void _bindTypeIdWithName(TypeId id,std::string const&name);
        TypeId _typeExists(
            DescriptionVector const&description,
            size_t                 &i);
        TypeId _typeIdExists(
            DescriptionVector const&description,
            size_t                 &i);
        TypeId _addType(
            std::string       const&name       ,
            DescriptionVector const&description,
            size_t                 &i          );
        TypeId _addTypeId(
            std::string       const&name       ,
            DescriptionVector const&description,
            size_t                 &i          );
        void _callConstructors(uint8_t*ptr,TypeId id)const;
        void _callDestructors(uint8_t*ptr,TypeId id)const;
        TypeDescription*_getDescription(TypeId id)const;
    };
  }
}

