#pragma once

#include<cassert>
#include<map>
#include<vector>
#include<iostream>
#include<functional>
#include<typeinfo>
#include<typeindex>
#include<tuple>

using Destructor = std::function<void(void*)>;

class NewData{
  public:
    NewData():type(typeid(void)){}
    template<typename CLASS,typename...ARGS>
      void create(ARGS ... args){
        assert(this != nullptr);

        this->destructor = [](void*ptr){
          static_cast<CLASS*>(ptr)->~CLASS();
          delete[](uint8_t*)ptr;
        };

        this->data = new uint8_t[sizeof(CLASS)];

        assert(this->data != nullptr);

        new(this->data)CLASS(args...);

        this->type = typeid(CLASS);
      }
    void*data;
    Destructor destructor;
    std::type_index type;
};


template<typename CLASS,typename...ARGS>
NewData newData(ARGS...args){
  NewData a{};
  a.create<CLASS,ARGS...>(args...);
  return a;
}

class AllContainer{
  public:
    using TypeData = std::tuple<Destructor,std::vector<void*>,std::map<std::string,void*>>;
    enum TypeDataParts{
      DESTRUCTOR = 0,
      VECTOR     = 1,
      MAP        = 2,
    };
    AllContainer(){}
    virtual ~AllContainer(){
      assert(this!=nullptr);
      for(auto const&x:this->_data){
        auto const&des = std::get<DESTRUCTOR>(x.second);
        auto const&vc  = std::get<VECTOR>(x.second);
        auto const&mp  = std::get<MAP>(x.second);
        for(auto const&y:vc)des(y);
        for(auto const&y:mp)des(y.second);
      }
    }
    bool hasType(std::type_index const&type)const{
      assert(this != nullptr);
      return this->_data.count(type) != 0;
    }
    bool hasValues(std::type_index const&type)const{
      assert(this != nullptr);
      if(!this->hasType(type))return false;
      return std::get<VECTOR>(this->_data.at(type)).size()!=0;
    }
    bool hasNamedValue(std::type_index const&type,std::string const&name)const{
      assert(this!=nullptr);
      if(!this->hasType(type))return false;
      return std::get<MAP>(this->_data.at(type)).count(name)!=0;
    }
    size_t getNofValues(std::type_index const&type)const{
      assert(this!=nullptr);
      assert(this->hasValues(type)==true);
      return std::get<VECTOR>(this->_data.at(type)).size();
    }
    void*getValue(std::type_index const&type,size_t index){
      assert(this!=nullptr);
      assert(index<this->getNofValues(type));
      return std::get<VECTOR>(this->_data.at(type)).at(index);
    }
    void*getNamedValue(std::type_index const&type,std::string const&name){
      assert(this!=nullptr);
      assert(this->hasNamedValue(type,name)==true);
      return std::get<MAP>(this->_data.at(type)).at(name);
    }
    std::vector<void*>const&getValues(std::type_index const&type){
      assert(this!=nullptr);
      assert(this->hasType(type));
      return std::get<VECTOR>(this->_data.at(type));
    }
    std::map<std::string,void*>const&getNamedValues(std::type_index const&type){
      assert(this!=nullptr);
      assert(this->hasType(type));
      return std::get<MAP>(this->_data.at(type));
    }


    void addType(std::type_index const&type,Destructor const&destructor){
      assert(this!=nullptr);
      if(this->hasType(type))return;
      this->_data[type] = TypeData(
          destructor,
          std::vector<void*>(),
          std::map<std::string,void*>());
    }
    size_t addValue(std::type_index const&type,void*data,Destructor const&destructor){
      assert(this!=nullptr);
      this->addType(type,destructor);
      size_t index = std::get<VECTOR>(this->_data.at(type)).size();
      std::get<VECTOR>(this->_data.at(type)).push_back(data);
      return index;
    }
    void addNamedValue(std::type_index const&type,std::string const&name,void*data,Destructor const&destructor){
      assert(this!=nullptr);
      assert(this->hasNamedValue(type,name)==false);
      this->addType(type,destructor);
      std::get<MAP>(this->_data.at(type))[name] = data;
    }
    size_t addValue(NewData const&data){
      assert(this!=nullptr);
      return this->addValue(data.type,data.data,data.destructor);
    }
    void removeValue(std::type_index const&type,size_t index){
      assert(this!=nullptr);
      if(index>=this->getNofValues(type))return;
      auto& val = this->_data.at(type);
      std::get<DESTRUCTOR>(val)(std::get<VECTOR>(val).at(index));
      std::get<VECTOR>(val).erase(std::get<VECTOR>(val).begin()+index);
    }
    void removeNamedValue(std::type_index const&type,std::string const&name){
      if(!this->hasNamedValue(type,name))return;
      auto& val = this->_data.at(type);
      std::get<DESTRUCTOR>(val)(std::get<MAP>(val).at(name));
      std::get<MAP>(val).erase(name);
    }


    template<typename CLASS>
      bool hasType()const{
        assert(this!=nullptr);
        return this->hasType(typeid(CLASS));
      }
    template<typename CLASS>
      bool hasValues()const{
        assert(this!=nullptr);
        return this->hasValues(typeid(CLASS));
      }
    template<typename CLASS>
      bool hasNamedValue(std::string const&name)const{
        assert(this!=nullptr);
        return this->hasNamedValue(typeid(CLASS),name);
      }
    template<typename CLASS>
      size_t getNofValues()const{
        assert(this!=nullptr);
        return this->getNofValues(typeid(CLASS));
      }
    template<typename CLASS>
      CLASS*getValue(size_t index){
        assert(this!=nullptr);
        return (CLASS*)this->getValue(typeid(CLASS),index);
      }
    template<typename CLASS>
      CLASS*getNamedValue(std::string const&name){
        assert(this!=nullptr);
        return (CLASS*)this->getNamedValue(typeid(CLASS),name);
      }
    template<typename CLASS>
      std::vector<void*>const&getValues(){
        assert(this!=nullptr);
        return this->getValues(typeid(CLASS));
      }
    template<typename CLASS>
      std::map<std::string,void*>const&getNamedValues(){
        assert(this!=nullptr);
        return this->getNamedValues(typeid(CLASS));
      }
    template<typename CLASS,typename...ARGS>
      static void*constructData(ARGS...args){
        void*newData = new uint8_t[sizeof(CLASS)];
        assert(newData!=nullptr);
        new(newData)CLASS(args...);
        return newData;
      }
    template<typename CLASS>
      void addType(){
        assert(this!=nullptr);
        this->addType(typeid(CLASS),[](void*ptr){((CLASS*)ptr)->~CLASS();delete[](uint8_t*)ptr;});
      }
    template<typename CLASS,typename...ARGS>
      size_t addValue(ARGS...args){
        assert(this!=nullptr);
        return this->addValue(
            typeid(CLASS),
            AllContainer::constructData<CLASS>(args...),
            [](void*ptr){((CLASS*)ptr)->~CLASS();delete[](uint8_t*)ptr;});
      }
    template<typename CLASS,typename...ARGS>
      void addNamedValue(std::string const&name,ARGS...args){
        assert(this!=nullptr);
        this->addNamedValue(
            typeid(CLASS),
            name,
            AllContainer::constructData<CLASS>(args...),
            [](void*ptr){((CLASS*)ptr)->~CLASS();delete[](uint8_t*)ptr;});
      }
    template<typename CLASS>
      void removeValue(size_t index){
        assert(this!=nullptr);
        this->removeValue(typeid(CLASS),index);
      }
    template<typename CLASS>
      void removeNamedValue(std::string const&name){
        assert(this!=nullptr);
        this->removeNamedValue(typeid(CLASS),name);
      }
  protected:
    std::map<std::type_index,TypeData>_data;
};


