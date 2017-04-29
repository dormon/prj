#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<memory>
#include<sstream>
#include<cstring>
#include<geCore/dtemplates.h>

#include<geCore/syntaxTree.h>

namespace ge{
  namespace core{

    class Syntax{
      public:
        NodeContext ctx;
        SyntaxNode::Node st_root=nullptr;
        std::string start;
        std::map<std::string,std::shared_ptr<Symbol>>name2Nonterm;
        std::map<TermType   ,std::shared_ptr<Symbol>>name2Term   ;

        std::shared_ptr<SyntaxNode>root = nullptr;
        std::shared_ptr<SyntaxNode>currentNode = nullptr;

        std::shared_ptr<Symbol>const&_addNonterm(std::string    name);
        std::shared_ptr<Symbol>const&_addTerm   (TermType const&type);

        std::map<TermType,std::string>_termNames;

        using Callback = void(*)(SyntaxNode::Node node,void*data);
        using Key      = std::pair<std::shared_ptr<Nonterm>,NontermNode::SideIndex>;
        using Value    = std::tuple<Callback,Callback,void*>;
        std::map<Key,Value>_callbacks;
        Range<TermIndex> _range;
        void prepareRun();
      public:
        Syntax(std::string start);
        ~Syntax();

        void processTree(std::shared_ptr<SyntaxNode>const&root);

        void runStart();
        NodeContext::Status runContinue();

        SyntaxNode::Node getSyntaxTree()const{
          return this->st_root;
        }

        void registerTermName(TermType term,std::string name){
          this->_termNames[term]=name;
        }

        template<typename...>
          void addTerm();
        template<typename...ARGS>
          void addTerm(TermType const&term,ARGS...args);

        void addRule(
            std::string nonterm,
            std::vector<std::weak_ptr<Symbol>>side);

        template<typename...>
          void addSymbol2Side(
              std::vector<std::weak_ptr<Symbol>>&side);
        template<typename...ARGS>
          void addSymbol2Side(
              std::vector<std::weak_ptr<Symbol>>&side,
              std::string sym,
              ARGS...args);
        template<typename...ARGS>
          void addSymbol2Side(
              std::vector<std::weak_ptr<Symbol>>&side,
              TermType sym,
              ARGS...args);
        template<typename...ARGS>
          void addSymbol2Side(
              std::vector<std::weak_ptr<Symbol>>&,
              Callback,ARGS...);

        template<typename...ARGS>
          void getCallback(Value&value,TermType,ARGS...);
        template<typename...ARGS>
          void getCallback(Value&value,std::string,ARGS...);
        template<typename...>
          void getCallback(Value&);
        template<typename...ARGS>
          void getCallback(Value&value,Callback,ARGS...);

        void createCallback(Value&value,Callback pre=nullptr,Callback post=nullptr,void*data=nullptr);

        void addCallback(std::string name,NontermNode::SideIndex side,Callback pre=nullptr,Callback post=nullptr,void*data=nullptr);


        template<typename...ARGS>
          void addRule(std::string nonterm,ARGS...args);

        void computeLengths();
        std::string str()const;
    };

    Syntax::Syntax(std::string start){
      this->start = start;
    }

    Syntax::~Syntax(){
      this->st_root=nullptr;
    }

    void Syntax::processTree(std::shared_ptr<SyntaxNode>const&root){
      if(!root->isNonterm())return;
      auto n = std::dynamic_pointer_cast<NontermNode>(root);
      auto v=this->_callbacks[Key(n->getNonterm(),n->side)];
      if(std::get<0>(v))std::get<0>(v)(root,std::get<2>(v));
      for(auto x:n->childs)
        this->processTree(x);
      if(std::get<1>(v))std::get<1>(v)(root,std::get<2>(v));
    }

    template<typename...>
      void Syntax::addTerm(){
      }

    template<typename...ARGS>
      void Syntax::addTerm(TermType const&term,ARGS...args){
        this->ctx.addTerm(term);
        this->addTerm(args...);
      }

    void Syntax::prepareRun(){
    }

    void Syntax::runStart(){
      this->ctx.terms.clear();
      this->_range.min() = 0;
      this->_range.max() = 0;//std::min(this->name2Nonterm[this->start]->minLength,this);
      std::shared_ptr<SyntaxNode>emptyNode=nullptr;
      this->st_root = std::make_shared<NontermNode>(emptyNode,0,this->_range,this->name2Nonterm[this->start],true);
      this->ctx.setNode(this->st_root);
      this->ctx.setStatus(NodeContext::WAITING);
      this->ctx.calledFromChildOrRecheck = false;
      this->ctx.termIndex = 0;
      this->ctx.virtualEnd = this->_range.max();
      this->ctx.currentLevel = 0;
    }

    NodeContext::Status Syntax::runContinue(){
      if(this->ctx.terms.size()<this->name2Nonterm[this->start]->range.min())
        return NodeContext::WAITING;

      TermIndex newEnd;
      if(this->_range.max()<this->name2Nonterm[this->start]->range.min())
        newEnd = this->name2Nonterm[this->start]->range.min();
      else
        newEnd = this->_range.max()+1;

      if(newEnd>this->ctx.terms.size())
        return NodeContext::WAITING;


      do{
        //std::cout<<"######################################################"<<std::endl;
        //std::cout<<"newEnd: "<<newEnd<<std::endl;
        auto node = this->ctx.getNode();
        while(node){
          node->range.max() = newEnd;
          node = node->parent.lock();
        }
        this->_range.max() = newEnd;
        this->ctx.getNode()->match(this->ctx);
        if(this->ctx.getStatus()==NodeContext::WAITING){
          if(this->_range.max()>=this->ctx.terms.size())
            return NodeContext::WAITING;
          newEnd = this->_range.max()+1;
          if(newEnd>=this->name2Nonterm[this->start]->range.max())
            return NodeContext::FALSE;
        }else return this->ctx.getStatus();
      }while(true);
    }


    std::shared_ptr<Symbol>const&Syntax::_addNonterm(std::string name){
      auto ii=this->name2Nonterm.find(name);
      if(ii!=this->name2Nonterm.end())
        return ii->second;
      return this->name2Nonterm[name]=std::make_shared<Nonterm>(name);
    }

    std::shared_ptr<Symbol>const& Syntax::_addTerm(TermType const&type){
      auto ii=this->name2Term.find(type);
      if(ii!=this->name2Term.end())
        return ii->second;
      auto jj=this->_termNames.find(type);
      if(jj!=this->_termNames.end())
        return this->name2Term[type]=std::make_shared<Term>(type,jj->second);
      return this->name2Term[type]=std::make_shared<Term>(type);
    }

    void Syntax::addRule(
        std::string nonterm,
        std::vector<std::weak_ptr<Symbol>>side){
      auto n=this->_addNonterm(nonterm);
      auto ptr=std::dynamic_pointer_cast<Nonterm>(n);
      ptr->addSide(side);
    }

    template<typename...>
      void Syntax::addSymbol2Side(
          std::vector<std::weak_ptr<Symbol>>&){
      }

    template<typename...ARGS>
      void Syntax::addSymbol2Side(
          std::vector<std::weak_ptr<Symbol>>&side,
          std::string sym,
          ARGS...args){
        side.push_back(this->_addNonterm(sym));
        this->addSymbol2Side(side,args...);
      }

    template<typename...ARGS>
      void Syntax::addSymbol2Side(
          std::vector<std::weak_ptr<Symbol>>&side,
          TermType sym,
          ARGS...args){
        side.push_back(this->_addTerm(sym));
        this->addSymbol2Side(side,args...);
      }

    template<typename...ARGS>
      void Syntax::addSymbol2Side(
          std::vector<std::weak_ptr<Symbol>>&,
          Callback,ARGS...){
      }

    template<typename...ARGS>
      void Syntax::getCallback(Value&value,TermType,ARGS...args){
        this->getCallback(value,args...);
      }

    template<typename...ARGS>
      void Syntax::getCallback(Value&value,std::string,ARGS...args){
        this->getCallback(value,args...);
      }

    template<typename...>
      void Syntax::getCallback(Value&){
      }

    template<typename...ARGS>
      void Syntax::getCallback(Value&value,Callback a,ARGS...args){
        this->createCallback(value,a,args...);
      }

    void Syntax::createCallback(Value&value,Callback pre,Callback post,void*data){
      std::get<0>(value)=pre ;
      std::get<1>(value)=post;
      std::get<2>(value)=data;
    }

    void Syntax::addCallback(std::string name,NontermNode::SideIndex side,Callback pre,Callback post,void*data){
      auto v=Value(pre,post,data);
      auto k=Key(std::dynamic_pointer_cast<Nonterm>(this->name2Nonterm[name]),side);
      this->_callbacks[k]=v;
    }

    template<typename...ARGS>
      void Syntax::addRule(std::string nonterm,ARGS...args){
        std::vector<std::weak_ptr<Symbol>>side;
        this->addSymbol2Side(side,args...);
        auto n=this->_addNonterm(nonterm);
        auto ptr=std::dynamic_pointer_cast<Nonterm>(n);
        auto sideIndex = ptr->addSide(side);
        Value v;
        this->getCallback(v,args...);
        this->addCallback(nonterm,sideIndex,std::get<0>(v),std::get<1>(v),std::get<2>(v));
      }


    std::string Syntax::str()const{
      std::stringstream ss;
      for(auto x:this->name2Nonterm){
        ss<<x.second->range.str();
        for(auto y:std::dynamic_pointer_cast<Nonterm>(x.second)->prefixes){
          ss<<" "<<this->name2Term.find(y)->second->name;
        }
        ss<<std::endl;
        ss<<x.second->str();
      }
      return ss.str();
    }

    void Syntax::computeLengths(){
      std::set<std::string>visited;
      //this->name2Nonterm[this->start]->computeMinLength(visited);

      std::set<std::shared_ptr<Nonterm>>allNonterms;
      for(auto x:this->name2Nonterm)
        allNonterms.insert(std::dynamic_pointer_cast<Nonterm>(x.second));
      computeMinLength(allNonterms);
      computeMaxLength(allNonterms);


      for(auto x:this->name2Nonterm)
        std::dynamic_pointer_cast<Nonterm>(x.second)->computeSideSizes();

      for(auto x:this->name2Nonterm)
        std::dynamic_pointer_cast<Nonterm>(x.second)->sortSides();

      for(auto x:this->name2Nonterm)
        std::dynamic_pointer_cast<Nonterm>(x.second)->computeTails();

      computePrefixes(allNonterms);
      /*
      visited.clear();
      std::dynamic_pointer_cast<Nonterm>(this->name2Nonterm[this->start])->computePrefixes(visited);
      visited.clear();
      */
    }
  }
}
