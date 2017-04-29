#pragma once

#include<iostream>
#include<vector>
#include<set>
#include<memory>
#include<limits>
#include<numeric>
#include<algorithm>
#include<sstream>

namespace ge{
  namespace core{

    template<typename TYPE>
    TYPE saturatedAdd(TYPE a,TYPE b){
      return (a>std::numeric_limits<TYPE>::max()-b)?std::numeric_limits<TYPE>::max():a+b;
    }

    template<typename TYPE>
    class Range: protected std::tuple<TYPE,TYPE>{
      public:
        enum Elements{
          MIN = 0,
          MAX = 1,
        };
        inline Range():std::tuple<TYPE,TYPE>(){std::get<MIN>(*this)=std::numeric_limits<TYPE>::min();std::get<MAX>(*this)=std::numeric_limits<TYPE>::min();}
        inline Range(TYPE const&a,TYPE const&b):std::tuple<TYPE,TYPE>(a,b){}
        inline TYPE length()const{return std::get<MAX>(*this) -std::get<MIN>(*this);}
        inline bool empty ()const{return std::get<MAX>(*this)==std::get<MIN>(*this);}
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
        inline bool operator==(Range const&r)const{return std::get<MIN>(*this)==std::get<MIN>(r)&&std::get<MAX>(*this)==std::get<MAX>(r);}
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
        virtual bool prefixMatch(std::vector<TermType>::iterator&begin,std::vector<TermType>::const_iterator end)const = 0;
    };

    class Nonterm: public Symbol{
      public:
        using SymbolList  = std::vector<std::weak_ptr<Symbol>>;
        using SymbolIndex = SymbolList::size_type;
        using Side        = std::tuple<Range<TermIndex>,SymbolList,std::vector<Range<TermIndex>>>;
        using SideIndex   = std::vector<Side>::size_type;
        enum SideElements{
          RANGE   = 0,
          SYMBOLS = 1,
          TAILS   = 2,
        };
        std::vector<Side>rightSides;
        bool fresh = false;
        std::set<TermType>prefixes;
      public:
        inline Nonterm(std::string name);
        virtual~Nonterm();
        inline SideIndex addSide(SymbolList const& side);
        virtual std::string str()const;
        void computePrefixes(std::set<std::string>&visited);
        void computeSideSizes();
        void sortSides();
        void computeTails();
        inline SymbolIndex nofSymbols(SideIndex side)const;
        inline std::shared_ptr<Symbol>getSymbol(SideIndex side,SymbolIndex symbol)const;
        inline Range<TermIndex>getTail(SideIndex side,SideIndex symbol)const;
        virtual bool prefixMatch(TermType const&term)const;
        virtual bool prefixMatch(std::vector<TermType>::iterator&begin,std::vector<TermType>::const_iterator end)const;
        Range<TermIndex>computeTailLength(SideIndex side,SymbolIndex symbol)const;
        TermIndex    computeTailMinLength(SideIndex side,SymbolIndex symbol)const;
        TermIndex    computeTailMaxLength(SideIndex side,SymbolIndex symbol)const;
    };

    class Term: public Symbol{
      public:
        TermType type;
        inline Term(TermType const&type,std::string name="");
        inline virtual~Term();
        virtual std::string str()const;
        virtual bool prefixMatch(TermType const&term)const;
        virtual bool prefixMatch(std::vector<TermType>::iterator&begin,std::vector<TermType>::const_iterator end)const;
    };



    Nonterm::Nonterm(std::string name){
      //this->minLength = 0;
      this->fresh = false;
      this->name = name;
    }

    Nonterm::~Nonterm(){
    }

    Nonterm::SideIndex Nonterm::addSide(SymbolList const& side){
      this->rightSides.push_back(Side(Range<TermIndex>(),side,std::vector<Range<TermIndex>>()));
      return this->rightSides.size()-1;
    }

    std::string Nonterm::str()const{
      std::stringstream ss;
      for(auto x:this->rightSides){
        ss<<"<"<<this->name<<"> ::=";
        ss<<" "<<std::get<RANGE>(x).str();
        for(auto y:std::get<SYMBOLS>(x)){
          ss<<" ";
          auto n=std::dynamic_pointer_cast<Nonterm>(y.lock());
          if(n)ss<<"<"<<n->name<<">";
          else ss<<y.lock()->str();
        }
        ss<<std::endl;
      }
      return ss.str();
    }

    //A
    //A->B1
    //A->1
    //B->A2
    //
    //A 1-oo
    //B 2-oo
    //
    //A
    //A->B1
    //A->C
    //C->D
    //D->E
    //E->1
    //B->A2
    //
    //
    //C 
    //C->A1
    //C->111
    //A->B1
    //A->1111111
    //B->C2
    //
    //
    //A->B1
    //A->1
    //B->AC1
    //B->A
    //C->B1
    //
    //Algoritmus by mel jit odspodu od terminalnich pravidel
    //a postupne rozsirovat mnozinu nontermu, pro ktere uz znam min delku
    //
    //{A}
    //{A,B}
    //{A,B,C}
    //{A,B,C}

    bool computeMinLength(std::set<std::shared_ptr<Nonterm>>const&allNonterms){
      for(auto x:allNonterms){
        x->range.min()=std::numeric_limits<TermIndex>::max();
        x->range.max()=std::numeric_limits<TermIndex>::min();
      }
      std::set<std::shared_ptr<Nonterm>>terminating;
      std::set<std::shared_ptr<Nonterm>>newTerminating;
      do{
        terminating = newTerminating;
        newTerminating.clear();
        for(auto x:allNonterms){
          bool ignoreNonterm = true;
          for(auto y:x->rightSides){
            bool ignoreSide = false;
            for(auto z:std::get<Nonterm::SYMBOLS>(y)){
              auto sz = z.lock();
              if(std::dynamic_pointer_cast<Term>(sz))continue;
              if(terminating.find(std::dynamic_pointer_cast<Nonterm>(sz))!=terminating.end())continue;
              ignoreSide = true;
              break;
            }
            if(ignoreSide)continue;
            ignoreNonterm = false;
            TermIndex len=0;
            for(auto z:std::get<Nonterm::SYMBOLS>(y)){
              auto sz = z.lock();
              if(std::dynamic_pointer_cast<Term>(sz)){
                len+=1;
                continue;
              }
              len+=std::dynamic_pointer_cast<Nonterm>(sz)->range.min();
            }
            x->range.min() = std::min(x->range.min(),len);
          }
          if(!ignoreNonterm)
            newTerminating.insert(x);
        }
      }while(terminating!=newTerminating);
      if(terminating!=allNonterms){
        std::cerr<<"ERROR: computeMinLength() these nonterms: "<<std::endl;
        for(auto x:allNonterms)
          if(terminating.find(x)==terminating.end())
            std::cerr<<x->name<<" ";
        std::cerr<<"do not produce terminating strings"<<std::endl;
        return false;
      }
      return true;
    }

    void computePrefixes(std::set<std::shared_ptr<Nonterm>>const&allNonterms){
      std::set<std::shared_ptr<Nonterm>>hasPrefix;
      std::set<std::shared_ptr<Nonterm>>newPrefix;
      do{
        hasPrefix = newPrefix;
        newPrefix.clear();
        for(auto x:allNonterms){
          bool ignoreNonterm = false;
          for(auto y:x->rightSides){
            auto z=std::get<Nonterm::SYMBOLS>(y)[0].lock();
            if(std::dynamic_pointer_cast<Term>(z))continue;
            if(x==std::dynamic_pointer_cast<Nonterm>(z))continue;
            if(hasPrefix.find(std::dynamic_pointer_cast<Nonterm>(z))!=hasPrefix.end())continue;
            ignoreNonterm = true;
            break;
          }
          if(ignoreNonterm)continue;
          for(auto y:x->rightSides){
            auto z=std::get<Nonterm::SYMBOLS>(y)[0].lock(); 
            if(std::dynamic_pointer_cast<Term>(z))x->prefixes.insert(std::dynamic_pointer_cast<Term>(z)->type);
            else{
              if(std::dynamic_pointer_cast<Nonterm>(z)==x)continue;
              for(auto p:std::dynamic_pointer_cast<Nonterm>(z)->prefixes)
                x->prefixes.insert(p);
            }
          }
          newPrefix.insert(x);
        }
      }while(hasPrefix!=newPrefix);
    }

    TermIndex computeMaxLength(std::shared_ptr<Nonterm>const&n,std::set<std::string>&visited){
      if(visited.find(n->name)!=visited.end())return std::numeric_limits<TermIndex>::max();
      visited.insert(n->name);
      TermIndex longest=0;
      for(auto x:n->rightSides){
        TermIndex currentLen = 0;
        for(auto y:std::get<Nonterm::SYMBOLS>(x)){
          auto sy=y.lock();
          if(std::dynamic_pointer_cast<Term>(sy)){
            currentLen+=1;
            continue;
          }
          currentLen = saturatedAdd(currentLen,computeMaxLength(std::dynamic_pointer_cast<Nonterm>(sy),visited));
          if(currentLen == std::numeric_limits<TermIndex>::max())break;
        }
        longest = std::max(longest,currentLen);
        if(longest == std::numeric_limits<TermIndex>::max())break;
      }
      return longest;
    }

    void computeMaxLength(std::set<std::shared_ptr<Nonterm>>const&allNonterms){
      for(auto x:allNonterms){
        std::set<std::string>visited;
        x->range.max()=computeMaxLength(x,visited);
      }
    }

    void Nonterm::computePrefixes(std::set<std::string>&visited){
      if(visited.count(this->name))return;
      visited.insert(this->name);

      for(auto x:this->rightSides){
        auto sym = std::get<SYMBOLS>(x)[0].lock();
        auto nsym = std::dynamic_pointer_cast<Nonterm>(sym);
        if(nsym){
          nsym->computePrefixes(visited);
          for(auto y:nsym->prefixes)
            this->prefixes.insert(y);
        }else
          this->prefixes.insert(std::dynamic_pointer_cast<Term>(sym)->type);
      }
    }

    bool Nonterm::prefixMatch(TermType const&term)const{
      return this->prefixes.find(term)!=this->prefixes.end();
    }

    bool Nonterm::prefixMatch(std::vector<TermType>::iterator&begin,std::vector<TermType>::const_iterator end)const{
      using II = decltype(begin);
      if(begin==end)return false;

      auto ib = begin;
      auto ie = end;

      for(auto x:this->rightSides){
        ib = begin;
        ie = end;
        std::vector<std::pair<II,II>>parts;
        parts.push_back(std::pair<II,II>(ib,ib));
        parts[0].second++;

        bool sideMatch = true;
        for(auto y:std::get<SYMBOLS>(x)){
          auto sym = y.lock();
          if(!sym->prefixMatch(*begin)){
            sideMatch = false;
            break;
          }

        }
        if(!sideMatch)continue;
        if(begin==end)return true;
      }
      return false;
    }

    Range<TermIndex>Nonterm::computeTailLength(SideIndex side,SymbolIndex symbol)const{
      Range<TermIndex>result;
      for(SymbolIndex i=symbol+1;i<std::get<SYMBOLS>(this->rightSides[side]).size();++i){
        result.min()+=std::get<SYMBOLS>(this->rightSides[side])[i].lock()->range.min();
        result.max()=saturatedAdd(result.max(),std::get<SYMBOLS>(this->rightSides[side])[i].lock()->range.max());
      }
      return result;
    }

    void Nonterm::computeSideSizes(){
      for(auto&x:this->rightSides){
        std::get<RANGE>(x).min() = 0;
        for(auto y:std::get<SYMBOLS>(x)){
          std::get<RANGE>(x).min()+=y.lock()->range.min();
          std::get<RANGE>(x).max()=saturatedAdd(std::get<RANGE>(x).max(),y.lock()->range.max());
        }
      }
    }

    void Nonterm::sortSides(){
      std::sort(
          this->rightSides.begin(),
          this->rightSides.end(),
          [](Side const&a,Side const&b){
          return std::get<RANGE>(a).min()<std::get<RANGE>(b).min();
          });
    }

    void Nonterm::computeTails(){
      for(SideIndex i=0;i<this->rightSides.size();++i)
        for(SymbolIndex j=0;j<this->nofSymbols(i);++j)
          std::get<TAILS>(this->rightSides[i]).push_back(this->computeTailLength(i,j));
    }

    Nonterm::SymbolIndex Nonterm::nofSymbols(SideIndex side)const{
      return std::get<SYMBOLS>(this->rightSides[side]).size();
    }

    std::shared_ptr<Symbol>Nonterm::getSymbol(SideIndex side,SymbolIndex symbol)const{
      return std::get<SYMBOLS>(this->rightSides[side])[symbol].lock();
    }

    Range<TermIndex>Nonterm::getTail(SideIndex side,SideIndex symbol)const{
      return std::get<TAILS>(this->rightSides[side])[symbol];
    }

    Term::Term(TermType const&type,std::string name){
      this->range.min()=1;
      this->range.max()=1;
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

    bool Term::prefixMatch(TermType const&term)const{
      return this->type == term;
    }
    bool Term::prefixMatch(std::vector<TermType>::iterator&begin,std::vector<TermType>::const_iterator end)const{
      if(begin==end)return false;
      if(this->type ==*begin)begin++;
      else return false;
      if(begin!=end){
        begin--;
        return false;
      }
      return true;
    }
  }
}
