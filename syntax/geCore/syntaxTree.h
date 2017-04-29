#pragma once

#include<vector>
#include<memory>
#include"grammar.h"

namespace ge{
  namespace core{

    class SyntaxNode;
    class NontermNode;
    class TermNode;

    class NodeContext{
      public:
        using TermList  = std::vector<TermType>;
        using TermIndex = TermList::size_type;
        using Node      = std::shared_ptr<SyntaxNode>;
        DEF_ENUM(Status,TRUE,WAITING,FALSE);

        Status     status                  ;
        bool       calledFromChildOrRecheck;
        TermList   terms                   ;
        TermIndex  termIndex               ;
        Node       currentNode             ;
        inline NodeContext(Node const&currentNode = nullptr);
        inline void setNode(Node const&currentNode = nullptr);
        inline Node const&getNode()const;
        inline void addTerm(TermType const&term);
        inline bool end()const;
        inline TermType const&getTerm()const;
        inline void next();
        inline void setStatus(Status status);
        inline Status getStatus()const;
    };

    class SyntaxNode: public std::enable_shared_from_this<SyntaxNode>{
      public:
        using Parent       = std::weak_ptr<SyntaxNode>;
        using SharedSymbol = std::shared_ptr<Symbol>;
        using TermIndex    = NodeContext::TermIndex;
        using Node         = std::shared_ptr<SyntaxNode>;
        using ChildList    = std::vector<Node>;
        using ChildIndex   = ChildList::size_type;
        Parent       parent       ;
        ChildIndex   childInParent;
        SharedSymbol symbol       ;
        TermIndex    startTerm    ;
        SyntaxNode(
            Parent       const& parent       ,
            ChildIndex   const& childInParent,
            TermIndex    const& startTerm    ,
            SharedSymbol const& symbol       );
        virtual ~SyntaxNode();
        virtual void match(NodeContext&ctx)=0;
        inline void parentMatch(NodeContext&ctx);
        inline std::shared_ptr<Nonterm>getNonterm()const;
        inline std::shared_ptr<Term>   getTerm   ()const;
        inline bool isNonterm()const;
        inline bool areWeCurrent(NodeContext&ctx)const;
        virtual std::string str()const = 0;
    };

    class NontermNode: public SyntaxNode{
      public:
        using SideIndex = decltype(Nonterm::rightSides)::size_type;
        using ItemIndex = decltype(Nonterm::rightSides)::value_type::size_type;
        SideIndex side  ;
        ItemIndex item  ;
        ChildList childs;
        inline NontermNode(
            Parent       const& parent       ,
            ChildIndex   const& childInParent,
            TermIndex    const& startTerm    ,
            SharedSymbol const& symbol       );
        inline virtual ~NontermNode();
        inline Node const&lastChild()const;
        virtual void match(NodeContext&ctx);
        virtual std::string str()const;
    };

    class TermNode: public SyntaxNode{
      public:
        TermNode(
            Parent       const& parent       ,
            ChildIndex   const& childInParent,
            TermIndex    const& startTerm    ,
            SharedSymbol const& symbol       );
        virtual ~TermNode();
        virtual void match(NodeContext&ctx);
        virtual std::string str()const;
    };



    NodeContext::NodeContext(NodeContext::Node const&currentNode){
      this->currentNode              = currentNode;
      this->termIndex                = 0          ;
      this->calledFromChildOrRecheck = false      ;
    }

    void NodeContext::setNode(NodeContext::Node const&currentNode){
      this->currentNode = currentNode;
    }

    NodeContext::Node const&NodeContext::getNode()const{
      return this->currentNode;
    }

    void NodeContext::addTerm(TermType const&term){
      this->terms.push_back(term);
    }

    bool NodeContext::end()const{
      return this->termIndex>=this->terms.size();
    }

    TermType const&NodeContext::getTerm()const{
      return this->terms[this->termIndex];
    }

    void NodeContext::next(){
      this->termIndex++;
    }

    void NodeContext::setStatus(Status status){
      this->status = status;
    }

    NodeContext::Status NodeContext::getStatus()const{
      return this->status;
    }



    SyntaxNode::SyntaxNode(
        Parent       const& parent       ,
        ChildIndex   const& childInParent,
        TermIndex    const& startTerm    ,
        SharedSymbol const& symbol       ){
      this->parent        = parent       ;
      this->childInParent = childInParent;
      this->startTerm     = startTerm    ;
      this->symbol        = symbol       ;
    }

    SyntaxNode::~SyntaxNode(){
    }


    inline void SyntaxNode::parentMatch(NodeContext&ctx){
      ctx.calledFromChildOrRecheck = true;
      auto p=this->parent.lock();
      if(p)p->match(ctx);
    }

    std::shared_ptr<Nonterm>SyntaxNode::getNonterm()const{
      return std::dynamic_pointer_cast<Nonterm>(this->symbol);
    }

    std::shared_ptr<Term>   SyntaxNode::getTerm   ()const{
      return std::dynamic_pointer_cast<Term>(this->symbol);
    }

    bool SyntaxNode::isNonterm()const{
      return this->getNonterm()!=nullptr;
    }

    bool SyntaxNode::areWeCurrent(NodeContext&ctx)const{
      return this->shared_from_this()==ctx.getNode();
    }



    NontermNode::NontermNode(
        Parent       const& parent       ,
        ChildIndex   const& childInParent,
        TermIndex    const& startTerm    ,
        SharedSymbol const& symbol       ):
      SyntaxNode(
          parent       ,
          childInParent,
          startTerm    ,
          symbol       ){
        this->side = 0;
        this->item = 0;
      }

    NontermNode::~NontermNode(){
    }

    NontermNode::Node const&NontermNode::lastChild()const{
      return this->childs[this->childs.size()-1];
    }

    void NontermNode::match(NodeContext&ctx){
      bool cfc=ctx.calledFromChildOrRecheck;
      bool cfc2=cfc;
      ctx.calledFromChildOrRecheck=false;
      while(true){
        if(!cfc2){
          auto n=this->getNonterm();
          auto subsym=n->rightSides[this->side][this->item].lock();
          std::shared_ptr<SyntaxNode>newNode;
          if(std::dynamic_pointer_cast<Nonterm>(subsym))
            newNode = std::make_shared<NontermNode>(
                this->shared_from_this(),
                this->childs.size(),
                ctx.termIndex,
                subsym);
          else
            newNode = std::make_shared<TermNode>(
                this->shared_from_this(),
                this->childs.size(),
                ctx.termIndex,
                subsym);
          this->childs.push_back(newNode);
          this->lastChild()->match(ctx);
        }
        cfc2 = false;

        switch(ctx.getStatus()){
          case NodeContext::TRUE:
            if(this->item+1==this->getNonterm()->rightSides[this->side].size()){
              if(cfc)this->parentMatch(ctx);
              return;
            }
            this->item++;
            break;
          case NodeContext::FALSE:
            this->childs.pop_back();
            while(
                !this->childs.empty() &&
                !this->lastChild()->isNonterm())
              this->childs.pop_back();
            if(this->childs.empty()){
              ctx.termIndex = this->startTerm;
              if(this->side+1==this->getNonterm()->rightSides.size()){
                if(cfc)this->parentMatch(ctx);
                return;
              }
              this->side++;
              this->item=0;
              break;
            }else{
              this->item = this->childs.size()-1;
              auto c=this->lastChild();
              while(c->isNonterm())
                c=std::dynamic_pointer_cast<NontermNode>(c)->lastChild();
              ctx.calledFromChildOrRecheck = true;
              c->match(ctx);
              return;
            }
            break;
          case NodeContext::WAITING:
            return;
        }
      }
    }

    std::string NontermNode::str()const{
      std::stringstream ss;
      ss<<"<"<<this->getNonterm()->name<<">"<<"(";
      for(ChildIndex i=0;i<this->childs.size();++i)
        ss<<this->childs[i]->str();
      ss<<")";
      return ss.str();
    }

    TermNode::TermNode(
        Parent       const& parent       ,
        ChildIndex   const& childInParent,
        TermIndex    const& startTerm    ,
        SharedSymbol const& symbol       ):
      SyntaxNode(
          parent       ,
          childInParent,
          startTerm    ,
          symbol       ){
      }

    TermNode::~TermNode(){
    }

    void TermNode::match(NodeContext&ctx){
      if(ctx.calledFromChildOrRecheck){
        ctx.setStatus(NodeContext::FALSE);
        ctx.termIndex = this->startTerm;
        this->parentMatch(ctx);
        return;
      }
      if(ctx.end()){
        ctx.setNode(this->shared_from_this());
        ctx.setStatus(NodeContext::WAITING);
        return;
      }
      if(ctx.getTerm()!=this->getTerm()->type){
        ctx.setStatus(NodeContext::FALSE);
      }else{
        ctx.next();
        ctx.setStatus(NodeContext::TRUE);
      }
      if(this->areWeCurrent(ctx))
        this->parentMatch(ctx);
    }

    std::string TermNode::str()const{
      std::stringstream ss;
      ss<<this->getTerm()->str();
      return ss.str();
    }

  }
}
