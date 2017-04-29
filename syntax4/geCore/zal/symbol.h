#pragma once

#include<iostream>
#include<limits>
#include<sstream>
#include<memory>
#include<vector>

namespace ge{
  namespace core{

    template<typename TYPE>
    TYPE saturatedAdd(TYPE a,TYPE b){
      return (a>std::numeric_limits<TYPE>::max()-b)?
        std::numeric_limits<TYPE>::max():a+b;
    }

    template<typename TYPE>
    class Range: protected std::tuple<TYPE,TYPE>{
      public:
        enum Elements{
          MIN = 0,
          MAX = 1,
        };
        inline Range():std::tuple<TYPE,TYPE>(){
          this->min() = std::numeric_limits<TYPE>::min();
          this->max() = std::numeric_limits<TYPE>::min();
        }
        inline Range(TYPE const&a,TYPE const&b):std::tuple<TYPE,TYPE>(a,b){}
        inline TYPE length()const{return this->max() -this->min();}
        inline bool empty ()const{return this->max()==this->min();}
        inline TYPE &min(){return std::get<MIN>(*this);}
        inline TYPE &max(){return std::get<MAX>(*this);}
        inline TYPE const&min()const{return std::get<MIN>(*this);}
        inline TYPE const&max()const{return std::get<MAX>(*this);}
        inline std::string str()const{
          std::stringstream ss;
          ss<<"["<<this->min()<<"-";
          if(this->max()==std::numeric_limits<TYPE>::max())ss<<"max";
          else ss<<this->max();
          ss<<"]";
          return ss.str();}
        inline bool operator==(Range const&r)const{
          return this->min()==r.min() && this->max()==r.max();
        }
        inline bool operator<(Range const&r)const{
          if(this->min()<r.min())return true;
          if(this->min()>r.min())return false;
          if(this->max()<r.max())return true;
          return false;
        }
    };

    using TermType = size_t;
    using TermList = std::vector<TermType>;
    using TermIndex = TermList::size_type;

    class Term;

    class Symbol: public std::enable_shared_from_this<Symbol>{
      public:
        std::string name = "";
        Range<TermIndex>range;
        virtual std::string str()const = 0;
        virtual bool prefixMatch(TermType const&term)const = 0;
    };
  }
}

