#pragma once

#include<iostream>
#include<vector>
#include<set>
#include<memory>
#include<limits>

namespace ge{
  namespace core{

    using TermType = size_t;

    class Symbol{
      public:
        std::string name = "";
        unsigned minLength = 0;
        bool fresh = false;
        virtual std::string str()const = 0;
        virtual bool computeMinLength(std::set<std::string>&visited) = 0;
    };

    class Nonterm: public Symbol{
      public:
        using Side      = std::vector<std::weak_ptr<Symbol>>;
        using SideIndex = Side::size_type;
        std::vector<Side>rightSides;
        inline Nonterm(std::string name);
        virtual~Nonterm();
        inline SideIndex addSide(Side side);
        virtual std::string str()const;
        virtual bool computeMinLength(std::set<std::string>&visited);
    };

    class Term: public Symbol{
      public:
        TermType type;
        inline Term(TermType const&type,std::string name="");
        inline virtual~Term();
        virtual std::string str()const;
        virtual bool computeMinLength(std::set<std::string>&visited);
    };



    Nonterm::Nonterm(std::string name){
      this->minLength = 0;
      this->fresh = false;
      this->name = name;
    }

    Nonterm::~Nonterm(){
    }

    Nonterm::SideIndex Nonterm::addSide(Side side){
      this->rightSides.push_back(side);
      return this->rightSides.size()-1;
    }

    std::string Nonterm::str()const{
      std::stringstream ss;
      for(auto x:this->rightSides){
        ss<<"<"<<this->name<<"> ::=";
        for(auto y:x){
          ss<<" ";
          auto n=std::dynamic_pointer_cast<Nonterm>(y.lock());
          if(n)ss<<"<"<<n->name<<">";
          else ss<<y.lock()->str();
        }
        ss<<std::endl;
      }
      return ss.str();
    }

    bool Nonterm::computeMinLength(std::set<std::string>&visited){
      if(this->fresh)return true;
      if(visited.count(this->name))return false;
      visited.insert(this->name);

      SideIndex leastIndex = this->rightSides.size();
      SideIndex currentIndex = 0;
      unsigned least = std::numeric_limits<unsigned>::max();
      for(auto x:this->rightSides){
        unsigned current = 0;
        bool ignore = false;
        for(auto y:x){
          if(!y.lock()->computeMinLength(visited)){
            ignore = true;
            break;
          }
          current+=y.lock()->minLength;
        }
        if(!ignore&&current<least){
          least = current;
          leastIndex = currentIndex;
        }
        currentIndex++;
      }
      if(leastIndex == this->rightSides.size())return false;
      this->minLength = least;
      this->fresh = true;
      visited.erase(this->name);
      return true;
    }

    Term::Term(TermType const&type,std::string name){
      this->minLength = 0;
      this->fresh = false;
      this->name = name;
      this->type = type;
    }

    Term::~Term(){
    }

    std::string Term::str()const{
      std::stringstream ss;
      if(this->name=="")
        ss<<"\""<<this->type<<"\"";
      else
        ss<<"\""<<this->name<<"\"";
      return ss.str();
    }

    bool Term::computeMinLength(std::set<std::string>&){
      this->minLength = 1;
      this->fresh = true;
      return true;
    }

  }
}
