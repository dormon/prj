#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<memory>
#include<cassert>
#include<geCore/ErrorPrinter.h>
#include<geCore/Text.h>
#include"Interval.h"

template<typename TYPE>
class ValueDomain{
  public:
    virtual ~ValueDomain(){}
    virtual bool contain(TYPE const&value)const = 0;
    virtual std::string toStr()const = 0;
};

template<typename TYPE>
class SimpleValueDomain: public ValueDomain<TYPE>{
  public:
    SimpleValueDomain(){}
    ~SimpleValueDomain(){}
    void addInterval(Interval<TYPE>const&interval){
      assert(this!=nullptr);
      std::vector<Interval<TYPE>>nonJoinable;
      Interval<TYPE>joined = interval;
      for(auto const&x:this->_intervals)
        if(x.second.isJoinable(interval))joined = joined.join(x.second);
        else nonJoinable.push_back(x.second);
      this->_intervals.clear();
      this->_intervals[joined.getLeft()] = joined;
      for(auto const&x:nonJoinable)
        this->_intervals[x.getLeft()] = x;
    }
    void addInterval(TYPE const&leftEndPoint,TYPE const&rightEndPoint,typename Interval<TYPE>::IntervalType const&type = Interval<TYPE>::CLOSED){
      assert(this!=nullptr);
      this->addInterval(Interval<TYPE>(leftEndPoint,rightEndPoint,type));
    }
    void addInterval(TYPE const&value){
      assert(this!=nullptr);
      this->addInterval(value,value);
    }
    void addIntervals(std::vector<Interval<TYPE>>const&intervals){
      assert(this!=nullptr);
      for(auto const&x:intervals)
        this->addInterval(x);
    }
    virtual bool contain(TYPE const&value)const override{
      assert(this!=nullptr);
      for(auto const&x:this->_intervals)
        if(x.second.contain(value))return true;
      return false;
    }
    virtual std::string toStr()const override{
      assert(this!=nullptr);
      bool first = true;
      std::stringstream ss;
      for(auto const&x:this->_intervals){
        if(first)first = false;
        else ss<<",";
        ss<<x.second.toStr();
      }
      return ss.str();
    }
  protected:
    std::map<TYPE,Interval<TYPE>> _intervals;
};



class ArgumentFormat{
  public:
    std::string name;
    enum ArgumentFormatType{VECTOR,CONTEXT,}formatType;
    ArgumentFormat(std::string const&n,ArgumentFormatType const&f):name(n),formatType(f){}
    virtual ~ArgumentFormat(){}
    virtual bool check(std::vector<std::string>arguments,size_t&index)const = 0;
    virtual bool equal(std::shared_ptr<ArgumentFormat>const&)const = 0;
};

template<typename TYPE>
class ArgumentVector: public ArgumentFormat{
  public:
    std::vector<TYPE>defaults;
    std::shared_ptr<ValueDomain<TYPE>>domain;
    ArgumentVector(
        std::string                       const&name          ,
        std::vector<TYPE>                 const&defs          ,
        std::shared_ptr<ValueDomain<TYPE>>const&dom  = nullptr):ArgumentFormat(name,VECTOR),defaults(defs),domain(dom){}
    virtual ~ArgumentVector(){}
    virtual bool check(std::vector<std::string>arguments,size_t&index)const override{
      assert(this!=nullptr);
      size_t oldIndex = index;
      if(index>=arguments.size()){index = oldIndex;return false;}
      if(arguments.at(index)!=this->name){index = oldIndex;return false;}
      ++index;
      for(size_t j=0;j<this->maxLength;++j){
        if(!ge::core::isConvertibleToValue<TYPE>(arguments.at(index)))return true;
        if(domain){
          if(!domain->contain(this->convert(arguments.at(index)))){
            index = oldIndex;
            ge::core::printError(GE_CORE_FCENAME,"argument: "+arguments.at(index)+" is not contained in domain: "+this->dom->toStr(),arguments,index);
            return false;
          }
        }
        ++index;
      }
      return true;
    }
    TYPE convert(std::string const&argument){
      if(std::is_same<TYPE,float>::value)
        return std::atof(argument.c_str());
      if(std::is_integral<TYPE>::value)
        return std::atoi(argument.c_str());
      if(std::is_same<TYPE,std::string>::value)
        return argument;
      assert(false&&"type is not int, uint, float or string");
    }
    virtual bool equal(std::shared_ptr<ArgumentFormat>const&other)const override{
      assert(this!=nullptr);
      if(this->name != other->name)return false;
      auto o = std::dynamic_pointer_cast<ArgumentVector<TYPE>>(other);
      if(!o)return false;
      if(this->defaults!=o->defaults)return false;
    }
};

class ContextFormat: public ArgumentFormat{
  public:
    std::map<std::string,std::shared_ptr<ArgumentFormat>>parts;
    ContextFormat(std::string const&name = ""):ArgumentFormat(name,CONTEXT){}
    virtual ~ContextFormat(){}
    bool addFormat(std::shared_ptr<ArgumentFormat>const&format){
      assert(this!=nullptr);
      auto ii = this->parts.find(format->name);
      if(ii!=this->parts.end()){
        if(ii->second->equal(format))return true;
      }else return false;
      this->parts[format->name] = format;
      return true;
    }
    virtual bool check(std::vector<std::string>arguments,size_t&index)const override{
      assert(this!=nullptr);
      size_t oldIndex = index;
      if(index>=arguments.size()){index = oldIndex;return false;}
      if(arguments.at(index)!=this->name){index = oldIndex;return false;}
      ++index;
      if(index>=arguments.size()){index = oldIndex;return false;}
      if(arguments.at(index)!="{"){
        ge::core::printError(GE_CORE_FCENAME,"expected { after context name: "+this->name,arguments,index);
        index = oldIndex;
        return false;
      }
      ++index;
      std::set<std::string>notUsed;
      for(auto const&x:this->parts)notUsed.insert(x.first);
      for(;;){
        if(index>=arguments.size()){
          ge::core::printError(GE_CORE_FCENAME,"unexpected end of arguments, expected } at the end of context with name: "+this->name,arguments,index);
          index = oldIndex;
          return false;
        }
        if(arguments.at(index)=="}"){++index;return true;}
        if(notUsed.empty()){
          ge::core::printError(GE_CORE_FCENAME,"expected } at the end of context with name: "+this->name+" not: "+arguments.at(index),arguments,index);
          index = oldIndex;
          return false;
        }
        std::string arg;
        bool argUsed = false;
        for(auto const&x:notUsed){
          arg = x;
          if(this->parts.at(arg)->check(arguments,index))argUsed = true;
        }
        if(argUsed)notUsed.erase(arg);
      }
    }
    virtual bool equal(std::shared_ptr<ArgumentFormat>const&other)const override{
      assert(this!=nullptr);
      if(this->name != other->name)return false;
      auto o = std::dynamic_pointer_cast<ContextFormat>(other);
      if(!o)return false;
      if(this->parts.size() != o->parts.size())return false;
      for(auto const&x:this->parts){
        auto ii = o->parts.find(x.first);
        if(ii==o->parts.end())return false;
        if(!x.second->equal(ii->second))return false;
      }
      return true;
    }
};


