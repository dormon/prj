#pragma once

#include<cassert>
#include<map>
#include<vector>
#include<iostream>
#include<functional>


class AllContainer{
  public:
    AllContainer(){
      assert(this!=nullptr);
    }
    virtual ~AllContainer(){
      assert(this!=nullptr);
      for(auto const&x:this->_name2id){
        size_t id = x.second;
        for(auto const&y:this->_data.at(id))
          this->_id2destructor.at(id)(y);
      }
    }
    bool hasValues(size_t type)const{
      assert(this!=nullptr);
      auto ii = this->_data.find(type);
      if(ii==this->_data.end())return false;
      return ii->second.size()!=0;
    }
    size_t getNofValues(size_t type)const{
      assert(this!=nullptr);
      assert(this->_data.count(type)!=0);
      return this->_data.at(type).size();
    }
    void* getValuePtr(size_t type,size_t index){
      assert(this!=nullptr);
      assert(this->_data.count(type)!=0);
      assert(index<this->_data.at(type).size());
      return this->_data.at(type).at(index);
    }
    template<typename CLASS>
      bool hasValues()const{
        assert(this!=nullptr);
        size_t id = this->getType<CLASS>();
        return this->hasValues(id);
      }
    template<typename CLASS>
      size_t getNofValues()const{
        assert(this!=nullptr);
        size_t id = this->getType<CLASS>();
        return this->getNofValues(id);
      }
    template<typename CLASS>
      CLASS*getValue(size_t index){
        assert(this!=nullptr);
        size_t id = this->getType<CLASS>();
        return (CLASS*)this->getValuePtr(id,index);
      }
    template<typename CLASS>
      std::vector<void*>const&getValues(){
        assert(this!=nullptr);
        size_t id = this->getType<CLASS>();
        return this->_data.at(id);
      }
    template<typename CLASS>
      size_t getType()const{
        assert(this!=nullptr);
        assert(this->_name2id.count(std::string(typeid(CLASS).name()))!=0);
        return this->_name2id.at(std::string(typeid(CLASS).name()));
      }
    template<typename CLASS,typename...ARGS>
    size_t addValue(ARGS...args){
      assert(this!=nullptr);
      std::string name = typeid(CLASS).name();
      size_t id;
      if(this->_name2id.count(name)==0){
        id = this->_name2id.size();
        this->_name2id[name] = id;
        this->_id2destructor[id] = [](void*ptr){((CLASS*)ptr)->~CLASS();delete[](uint8_t*)ptr;};
      }else id = this->_name2id.at(name);
      if(this->_data.count(id)==0)
        this->_data[id] = std::vector<void*>();
      void*newData = new uint8_t[sizeof(CLASS)];
      new(newData)CLASS(args...);
      size_t index = this->_data.at(id).size();
      this->_data.at(id).push_back(newData);
      return index;
    }
    void removeValue(size_t type,size_t index){
      if(this->_data.count(type)==0)return;
      auto&data=this->_data.at(type);
      if(index>=data.size())return;
      this->_id2destructor.at(type)(data.at(index));
      data.erase(data.begin()+index);
      if(data.size()==0)
        this->_data.erase(type);
    }
  protected:
    std::map<std::string,size_t>_name2id;
    std::map<size_t,std::function<void(void*)>>_id2destructor;
    std::map<size_t,std::vector<void*>>_data;
};


