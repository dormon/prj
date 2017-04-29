        //reinterpret_cast<OUTPUT&>(*this->getOutputData()) = uber_call(this,this->_impl,typename std::make_index_sequence<sizeof...(ARGS)>{});
#pragma once

#include<memory>
#include<cassert>
#include<vector>
#include<map>
#include"keyword.h"
#include"Defines.h"
#include"AtomicDescription.h"
#include"ArrayDescription.h"
#include"StructDescription.h"
#include"PtrDescription.h"
#include"FceDescription.h"
#include"MemFceDescription.h"
#include"AnyDescription.h"
#include"VoidDescription.h"
#include"Resource.h"

class Resource;
class Any;

template<>inline std::string keyword<Any>(){return "Any";}

class TypeRegister: public std::enable_shared_from_this<TypeRegister>{
  public:

    template<typename T,typename std::enable_if<is_basic<T>::value && !std::is_same<T,void>::value,unsigned>::type = 0>
      TypeId addType(FcePtr c = nullptr,FcePtr d = nullptr){
        PRINT_CALL_STACK(c,d);
        assert(this!=nullptr);
        return this->addAtomicType(keyword<T>(),sizeof(T),c,d);
      }
    template<typename T,typename std::enable_if<is_basic<T>::value && std::is_same<T,void>::value,unsigned>::type = 0>
      TypeId addType(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return this->addVoidType(keyword<T>());
      }
    template<typename T,typename std::enable_if<std::is_array<T>::value && std::rank<T>::value==1,unsigned>::type = 0>
      TypeId addType(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return this->addArrayType(keyword<T>(),keyword<typename std::remove_extent<T>::type>(),std::extent<T>::value);
      }
    template<typename T,typename std::enable_if<std::is_array<T>::value && std::rank<T>::value!=1,unsigned>::type = 0>
      TypeId addType(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        auto id = this->addType<typename std::remove_extent<T>::type>();
        return this->addArrayType(keyword<T>(),id,std::extent<T>::value);
      }
    template<typename T,typename std::enable_if<std::is_function<T>::value,unsigned>::type = 0>
      TypeId addType(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return this->addFceType<typename FceReturnType<T>::type>(typename FceArgType<T>::type());
      }
    template<typename T,typename std::enable_if<std::is_member_function_pointer<T>::value,unsigned>::type = 0>
      TypeId addType(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return this->addMemFceType<typename MemFceReturnType<T>::type,typename MemFceClassType<T>::type>(typename MemFceArgType<T>::type());
      }
    template<typename T,typename std::enable_if<std::is_pointer<T>::value,unsigned>::type = 0>
      TypeId addType(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        using pure = typename std::remove_pointer<T>::type;
        return this->addPtrType(keyword<T>(),this->getTypeId(keyword<pure>()));
      }


    TypeId addVoidType(
        std::string const&name){
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      if(this->_name2id.count(name)!=0)return this->_name2id.at(name);
      TypeId id = this->_descriptions.size();
      auto des = std::make_shared<VoidDescription>(name);
      assert(des!=nullptr);
      this->_descriptions.push_back(des);
      this->_name2id[name]=id;
      return id;
    }
    TypeId addAtomicType(
        std::string const&name,
        size_t size,
        FcePtr c = nullptr,
        FcePtr d = nullptr){
      PRINT_CALL_STACK(name,size,c,d);
      assert(this!=nullptr);
      if(this->_name2id.count(name)!=0)return this->_name2id.at(name);
      TypeId id = this->_descriptions.size();
      auto des = std::make_shared<AtomicDescription>(name,size,c,d);
      assert(des!=nullptr);
      this->_descriptions.push_back(des);
      this->_name2id[name]=id;
      return id;
    }

    TypeId addArrayType(std::string const&name,std::string const&innerType,size_t size){
      PRINT_CALL_STACK(name,innerType,size);
      assert(this!=nullptr);
      return this->addArrayType(name,this->getTypeId(innerType),size);
    }
    TypeId addArrayType(std::string const&name,TypeId innerType,size_t size){
      PRINT_CALL_STACK(name,innerType,size);
      assert(this!=nullptr);
      if(this->_name2id.count(name)!=0)return this->_name2id.at(name);
      TypeId id = this->_descriptions.size();
      auto des = std::make_shared<ArrayDescription>(name,innerType,size);
      assert(des!=nullptr);
      this->_descriptions.push_back(des);
      this->_name2id[name]=id;
      return id;
    }

    template<typename OUTPUT,typename...ARGS>
      TypeId addFceType(std::tuple<ARGS...>const&){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return this->addFceType<OUTPUT,ARGS...>(keyword<OUTPUT(ARGS...)>());
      }
    template<typename OUTPUT,typename...ARGS>
      TypeId addFceType(std::string const&name){
        PRINT_CALL_STACK(name);
        assert(this!=nullptr);
        TypeId returnType = this->getTypeId(keyword<OUTPUT>());
        std::vector<TypeId>args;
        std::vector<std::string>n = {keyword<ARGS>()...};
        for(auto const&x:n)
          args.push_back(this->getTypeId(x));
        return this->addFceType(name,returnType,args);
      }

    template<typename OUTPUT,typename...ARGS>
      TypeId addFceType(std::string const&name,OUTPUT(*)(ARGS...)){
        PRINT_CALL_STACK(name);
        assert(this!=nullptr);
        return this->addFceType<OUTPUT,ARGS...>(name);
      }

    TypeId addFceType(std::string const&name,TypeId returnType,std::vector<TypeId>const&args){
      PRINT_CALL_STACK(name,returnType,args);
      assert(this!=nullptr);
      if(this->_name2id.count(name)!=0)return this->_name2id.at(name);
      TypeId id = this->_descriptions.size();
      auto des = std::make_shared<FceDescription>(name,returnType,args);
      assert(des!=nullptr);
      this->_descriptions.push_back(des);
      this->_name2id[name]=id;
      return id;
    }

    template<typename OUTPUT,typename CLASS,typename...ARGS>
      TypeId addMemFceType(std::tuple<ARGS...>const&){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return this->addMemFceType<OUTPUT,CLASS,ARGS...>(keyword<OUTPUT(CLASS::*)(ARGS...)>());
      }
    template<typename OUTPUT,typename CLASS,typename...ARGS>
      TypeId addMemFceType(std::string const&name){
        PRINT_CALL_STACK(name);
        assert(this!=nullptr);
        TypeId returnType = this->getTypeId(keyword<OUTPUT>());
        TypeId classType = this->getTypeId(keyword<CLASS>());
        std::vector<TypeId>args;
        std::vector<std::string>n = {keyword<ARGS>()...};
        for(auto const&x:n)
          args.push_back(this->getTypeId(x));
        return this->addMemFceType(name,returnType,classType,args);
      }


    template<typename OUTPUT,typename CLASS,typename...ARGS>
      TypeId addMemFceType(std::string const&name,OUTPUT(CLASS::*)(ARGS...)){
        PRINT_CALL_STACK(name);
        assert(this!=nullptr);
        return this->addMemFceType<OUTPUT,CLASS,ARGS...>(name);
      }

    TypeId addMemFceType(std::string const&name,TypeId returnType,TypeId classType,std::vector<TypeId>const&args){
      PRINT_CALL_STACK(name,returnType,classType,args);
      assert(this!=nullptr);
      if(this->_name2id.count(name)!=0)return this->_name2id.at(name);
      TypeId id = this->_descriptions.size();
      auto des = std::make_shared<MemFceDescription>(name,returnType,classType,args);
      assert(des!=nullptr);
      this->_descriptions.push_back(des);
      this->_name2id[name]=id;
      return id;
    }


    TypeId addStructType(std::string const&name,std::vector<TypeId>const&args){
      PRINT_CALL_STACK(name,args);
      assert(this!=nullptr);
      if(this->_name2id.count(name)!=0)return this->_name2id.at(name);
      TypeId id = this->_descriptions.size();
      auto des = std::make_shared<StructDescription>(name,args);
      assert(des!=nullptr);
      this->_descriptions.push_back(des);
      this->_name2id[name]=id;
      return id;
    }

    TypeId addPtrType(std::string const&name,TypeId innerType){
      PRINT_CALL_STACK(name,innerType);
      assert(this!=nullptr);
      if(this->_name2id.count(name)!=0)return this->_name2id.at(name);
      TypeId id = this->_descriptions.size();
      auto des = std::make_shared<PtrDescription>(name,innerType);
      assert(des!=nullptr);
      this->_descriptions.push_back(des);
      this->_name2id[name]=id;
      return id;
    }


    TypeId addAnyType(){
      PRINT_CALL_STACK();
      std::string name = keyword<Any>();
      assert(this!=nullptr);
      if(this->_name2id.count(name)!=0)return this->_name2id.at(name);
      TypeId id = this->_descriptions.size();
      auto des = std::make_shared<AnyDescription>(name);
      assert(des!=nullptr);
      this->_descriptions.push_back(des);
      this->_name2id[name]=id;
      return id;
    }

    template<typename T>
      std::shared_ptr<Resource>createResource(T const&val){
        PRINT_CALL_STACK(val);
        assert(this!=nullptr);
        auto res = this->createResource(keyword<T>());
        //std::memcpy(res->getData(),&val,sizeof(T));
        (T&)(*res) = val;
        return res;
      }
    template<typename T>
      std::shared_ptr<Resource>createResource(){
        PRINT_CALL_STACK();
        assert(this!=nullptr);
        return this->createResource(keyword<T>());
      }
    std::shared_ptr<Resource>createResource(std::string name){
      PRINT_CALL_STACK(name);
      assert(this!=nullptr);
      assert(this->_name2id.count(name)!=0);
      auto id = this->_name2id.at(name);
      assert(id<this->_descriptions.size());
      auto de = this->_descriptions.at(id);
      assert(de!=nullptr);
      return de->createResource(this->shared_from_this());
    }
    void constructResource(TypeId id,void*ptr){
      PRINT_CALL_STACK(id,ptr);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d = this->_descriptions.at(id); 
      assert(d!=nullptr);
      d->destroy(ptr,this->shared_from_this());
    }
    void destroyResource(TypeId id,void*ptr){
      PRINT_CALL_STACK(id,ptr);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d = this->_descriptions.at(id); 
      assert(d!=nullptr);
      d->destroy(ptr,this->shared_from_this());
    }
    void freeResource(TypeId id,void*ptr){
      PRINT_CALL_STACK(id,ptr);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d = this->_descriptions.at(id);
      assert(d!=nullptr);
      d->freeResource(ptr,this->shared_from_this());
    }
    TypeId getTypeId(std::string const&name)const{
      PRINT_CALL_STACK(name);
      assert(this!=nullptr);
      assert(this->_name2id.count(name)!=0);
      return this->_name2id.at(name);
    }
    size_t getByteSize(TypeId id)const{
      PRINT_CALL_STACK(id);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d = this->_descriptions.at(id); 
      assert(d!=nullptr);
      return d->byteSize(std::const_pointer_cast<TypeRegister>(this->shared_from_this()));
    };
    std::string str(TypeId id)const{
      PRINT_CALL_STACK(id);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d=this->_descriptions.at(id);
      assert(d!=nullptr);
      return d->str(std::const_pointer_cast<TypeRegister>(this->shared_from_this()));
    }
    size_t nofTypes()const{
      PRINT_CALL_STACK();
      assert(this!=nullptr);
      return this->_descriptions.size();
    }
    std::string typeIdName(TypeId id)const{
      PRINT_CALL_STACK(id);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d=this->_descriptions.at(id);
      assert(d!=nullptr);
      return this->_descriptions.at(id)->name;
    }
    TypeId getArrayInnerType(TypeId id)const{
      PRINT_CALL_STACK(id);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d=this->_descriptions.at(id);
      assert(d!=nullptr);
      assert(d->type == TypeDescription::ARRAY);
      auto ad = std::dynamic_pointer_cast<ArrayDescription>(d);
      assert(ad!=nullptr);
      return ad->innerType;
    }
    TypeId getFceReturnType(TypeId id)const{
      PRINT_CALL_STACK(id);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d=this->_descriptions.at(id);
      assert(d!=nullptr);
      assert(d->type == TypeDescription::FCE);
      auto ad = std::dynamic_pointer_cast<FceDescription>(d);
      assert(ad!=nullptr);
      return ad->returnType;
    }
    TypeId getFceArgType(TypeId id,size_t n)const{
      PRINT_CALL_STACK(id,n);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d=this->_descriptions.at(id);
      assert(d!=nullptr);
      assert(d->type == TypeDescription::FCE);
      auto ad = std::dynamic_pointer_cast<FceDescription>(d);
      assert(ad!=nullptr);
      assert(n<ad->argsTypes.size());
      return ad->argsTypes.at(n);
    }
    size_t getNofFceArgs(TypeId id)const{
      PRINT_CALL_STACK(id);
      assert(this!=nullptr);
      assert(id<this->_descriptions.size());
      auto d=this->_descriptions.at(id);
      assert(d!=nullptr);
      assert(d->type == TypeDescription::FCE);
      auto ad = std::dynamic_pointer_cast<FceDescription>(d);
      assert(ad!=nullptr);
      return ad->argsTypes.size();
    }
    bool isConvertible(TypeId to,TypeId from)const{
      PRINT_CALL_STACK(to,from);
      assert(this!=nullptr);
      if(to==from)return true;
      if(to==this->getTypeId(keyword<Any>()))return true;
      if(from==this->getTypeId(keyword<Any>()))return true;
      return false;
    }
  private:
    std::vector<std::shared_ptr<TypeDescription>>_descriptions;
    std::map<std::string,size_t>_name2id;
};

