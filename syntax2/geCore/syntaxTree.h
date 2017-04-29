#pragma once

#include<vector>
#include<memory>
#include<geCore/NodeContext.h>

namespace ge{
  namespace core{

    class SyntaxNode;
    class NontermNode;
    class TermNode;

    class SyntaxNode: public std::enable_shared_from_this<SyntaxNode>{
      public:
        using Parent       = std::weak_ptr<SyntaxNode>;
        using SharedSymbol = std::shared_ptr<Symbol>;
        using Node         = std::shared_ptr<SyntaxNode>;
        using ChildList    = std::vector<Node>;
        using ChildIndex   = ChildList::size_type;
        Parent          parent       ;
        ChildIndex      childInParent;
        SharedSymbol    symbol       ;
        Range<TermIndex>range        ;
        bool            canWait      ;
        SyntaxNode(
            Parent          const& parent       ,
            ChildIndex      const& childInParent,
            Range<TermIndex>const& range        ,
            SharedSymbol    const& symbol       ,
            bool            const& canWait      );
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
        using SideIndex = Nonterm::SideIndex;
        using SymbolIndex = Nonterm::SymbolIndex;
        SideIndex   side;
        SymbolIndex item;
        ChildList childs;
        std::vector<Range<TermIndex>>divisions;
        inline NontermNode(
            Parent          const& parent       ,
            ChildIndex      const& childInParent,
            Range<TermIndex>const& range        ,
            SharedSymbol    const& symbol       ,
            bool            const& canWait      );
        inline virtual ~NontermNode();
        inline Node const&lastChild()const;
        virtual void match(NodeContext&ctx);
        virtual std::string str()const;
    };

    class TermNode: public SyntaxNode{
      public:
        TermNode(
            Parent          const& parent       ,
            ChildIndex      const& childInParent,
            Range<TermIndex>const& range        ,
            SharedSymbol    const& symbol       ,
            bool            const& canWait      );
        virtual ~TermNode();
        virtual void match(NodeContext&ctx);
        virtual std::string str()const;
    };






    SyntaxNode::SyntaxNode(
        Parent          const& parent       ,
        ChildIndex      const& childInParent,
        Range<TermIndex>const& range        ,
        SharedSymbol    const& symbol       ,
        bool            const& canWait      ){
      this->parent        = parent       ;
      this->childInParent = childInParent;
      this->range         = range        ;
      this->symbol        = symbol       ;
      this->canWait       = canWait      ;
    }

    SyntaxNode::~SyntaxNode(){
    }


    inline void SyntaxNode::parentMatch(NodeContext&ctx){
      ctx.calledFromChildOrRecheck = true;
      auto p=this->parent.lock();
      bool moveLevel = ctx.currentLevel!=0;
      if(moveLevel)ctx.currentLevel--;
      if(p)p->match(ctx);
      if(moveLevel)ctx.currentLevel++;
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
        Parent          const& parent       ,
        ChildIndex      const& childInParent,
        Range<TermIndex>const& range        ,
        SharedSymbol    const& symbol       ,
        bool            const& canWait      ):
      SyntaxNode(
          parent       ,
          childInParent,
          range        ,
          symbol       ,
          canWait      ){
        this->side = 0;
        this->item = 0;
      }

    NontermNode::~NontermNode(){
    }

    NontermNode::Node const&NontermNode::lastChild()const{
      return this->childs[this->childs.size()-1];
    }

    void NontermNode::match(NodeContext&ctx){
      //___;
      //std::cerr<<this->range.str()<<std::endl;
      if(this->symbol->range.min()>this->range.length()&&!this->canWait){
        ctx.setStatus(NodeContext::FALSE);
        if(ctx.calledFromChildOrRecheck)
          this->parentMatch(ctx);
        return;
      }
      //___;
      if(!ctx.calledFromChildOrRecheck){
        auto c=this->parent.lock();
        while(c){
          if(c->getNonterm()==this->getNonterm()&&c->range==this->range){
            ctx.setStatus(NodeContext::FALSE);
            if(ctx.calledFromChildOrRecheck)
              this->parentMatch(ctx);
            return;
          }
          c=c->parent.lock();
        }
      }
      //___;
      if(this->symbol->range.max()<this->range.length()){
        ctx.setStatus(NodeContext::FALSE);
        if(ctx.calledFromChildOrRecheck)
          this->parentMatch(ctx);
        return;
      }
      //___;
      if(this->range.empty()){
        ctx.setStatus(NodeContext::WAITING);
        ctx.setNode(this->shared_from_this());
        return;
      }
      //___;
      if(!ctx.calledFromChildOrRecheck&&!this->symbol->prefixMatch(ctx.getTerm())){
        ctx.setStatus(NodeContext::FALSE);
        if(ctx.calledFromChildOrRecheck)
          this->parentMatch(ctx);
        return;
      }
      //___;
#define ppp std::cout<<__LINE__<<": "<<ctx.termIndex<<std::endl
      auto printStatus=[](NodeContext::Status status,unsigned currentLevel){
        return;
        std::string statusName[3]={"TRUE","WAITING","FALSE"};
        for(unsigned i=0;i<currentLevel;++i)
          std::cout<<"  ";
        std::cout<<statusName[status]<<std::endl;
      };

      bool cfc=ctx.calledFromChildOrRecheck;
      bool cfc2=cfc;
      if(this->areWeCurrent(ctx))
        cfc = true;



      ctx.calledFromChildOrRecheck=false;
      while(true){
        if(!cfc2){
          /*
          for(unsigned i=0;i<ctx.currentLevel;++i)
            std::cout<<"  ";
          std::cout<<"<"<<this->getNonterm()->name<<"> ->";
          decltype(this->item)counter=0;
          for(auto x:std::get<Nonterm::SYMBOLS>(this->getNonterm()->rightSides[this->side])){
            std::cout<<" ";
            if(counter==this->item)
              std::cout<<"(";
            std::cout<<x.lock()->name;
            if(counter==this->item)
              std::cout<<")";
            counter++;
          }
          std::cout<<" "<<this->range.str()<<" ";
          std::cout<<std::endl;
          */

          //ppp;
          auto n=this->getNonterm();
          auto subsym=n->getSymbol(this->side,this->item);
          std::shared_ptr<SyntaxNode>newNode;
          if(this->divisions.size()!=this->item+1){
            if(this->item+1==n->nofSymbols(this->side))
              this->divisions.push_back(Range<TermIndex>(ctx.termIndex,this->range.max()));
            else
              this->divisions.push_back(Range<TermIndex>(ctx.termIndex,ctx.termIndex+subsym->range.min()));
          }
          bool cWait=false;
          if(!this->canWait){
            if(this->range.max()<this->divisions.back().max()+this->getNonterm()->getTail(this->side,this->item).min()){
              if(this->range.max()<this->divisions.back().min()+this->getNonterm()->getTail(this->side,this->item).min())
                this->divisions.back().max()=this->divisions.back().min();
              else
                this->divisions.back().max()=this->range.max()-this->getNonterm()->getTail(this->side,this->item).min();
            }
          }else{
            if(this->range.max()<=this->divisions.back().max()){
              this->divisions.back().max()=this->range.max();
              cWait=this->canWait;
            }
          }

          if(std::dynamic_pointer_cast<Nonterm>(subsym))
            newNode = std::make_shared<NontermNode>(
                this->shared_from_this(),
                this->childs.size(),
                this->divisions.back(),
                subsym,
                cWait);
          else
            newNode = std::make_shared<TermNode>(
                this->shared_from_this(),
                this->childs.size(),
                this->divisions.back(),
                subsym,
                cWait);
          this->childs.push_back(newNode);
          ctx.currentLevel++;
          this->lastChild()->match(ctx);
          ctx.currentLevel--;
        }
        cfc2 = false;
        switch(ctx.getStatus()){
          case NodeContext::TRUE:
            //ppp;
            if(this->item+1==this->getNonterm()->nofSymbols(this->side)){
              /*
                 if(ctx.termIndex!=this->range.second){
                 while(
                 !this->childs.empty()&&
                 !this->childs.back()->isNonterm()){
                 this->childs   .pop_back();
                 this->divisions.pop_back();
                 }
                 if(this->childs.empty()){
                 ctx.termIndex = this->range.first;
                 if(this->side+1==this->getNonterm()->rightSides.size()){
                 ctx.setStatus(NodeContext::FALSE);
                 if(cfc)this->parentMatch(ctx);
                 printStatus(ctx.getStatus(),ctx.currentLevel);
                 return;
                 }
                 this->side++;
                 this->item=0;
                 break;
                 }else{
                 this->item = this->childs.size()-1;
                 auto c=this->lastChild();
                 while(c->isNonterm()){
                 ctx.currentLevel++;
                 c=std::dynamic_pointer_cast<NontermNode>(c)->lastChild();
                 }
                 ctx.calledFromChildOrRecheck = true;
                 ctx.currentLevel++;
                 c->match(ctx);
                 ctx.currentLevel--;
                 printStatus(ctx.getStatus(),ctx.currentLevel);
                 return;
                 }
                 }*/
              if(cfc)this->parentMatch(ctx);
              printStatus(ctx.getStatus(),ctx.currentLevel);
              return;
            }
            this->item++;
            break;
          case NodeContext::FALSE:

            do{
              if(!this->childs.empty())this->childs.pop_back();
              if(this->divisions.empty())break;
              this->divisions.back().max()++;
              auto len=this->divisions.back().length();
              auto maxLen = this->getNonterm()->getSymbol(this->side,this->item)->range.max();
              if(
                  len>maxLen||
                  //this->divisions.back().max()>this->range.max()||
                  (!this->canWait &&
                   this->range.max()<this->divisions.back().max()+this->getNonterm()->getTail(this->side,this->item).min())||
                  (this->canWait && 
                   this->range.max()<this->divisions.back().max())
                ){
                this->divisions.pop_back();
                if(this->item>0)this->item--;
                continue;
              }
              break;
            }while(true);

            if(this->divisions.empty()){
              ctx.termIndex = this->range.min();
              if(this->side+1==this->getNonterm()->rightSides.size()){
                if(cfc)this->parentMatch(ctx);
                printStatus(ctx.getStatus(),ctx.currentLevel);
                return;
              }
              this->side++;
              this->item=0;
            }else ctx.termIndex = this->divisions.back().min();
            break;
          case NodeContext::WAITING:
            printStatus(ctx.getStatus(),ctx.currentLevel);
            return;
        }
      }
      printStatus(ctx.getStatus(),ctx.currentLevel);
      return;
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
        Parent          const& parent       ,
        ChildIndex      const& childInParent,
        Range<TermIndex>const& range        ,
        SharedSymbol    const& symbol       ,
        bool            const& canWait      ):
      SyntaxNode(
          parent       ,
          childInParent,
          range        ,
          symbol       ,
          canWait      ){
      }

    TermNode::~TermNode(){
    }

    void TermNode::match(NodeContext&ctx){
      auto printStatus=[](NodeContext::Status status,unsigned currentLevel){
        return;
        std::string statusName[3]={"TRUE","WAITING","FALSE"};
        for(unsigned i=0;i<currentLevel;++i)
          std::cout<<"  ";
        std::cout<<statusName[status]<<std::endl;
      };

      /*
      for(unsigned i=0;i<ctx.currentLevel;++i)
        std::cout<<"  ";
      std::cout<<"\""<<this->getTerm()->name<<"\" ";
      std::cout<<" ["<<this->range.min()<<"-"<<this->range.max()<<"]";
      if(ctx.calledFromChildOrRecheck)std::cout<<" recheck";
      std::cout<<std::endl;
      */

      if(this->range.empty()){
        if(this->canWait){
          ctx.setNode(this->shared_from_this());
          ctx.setStatus(NodeContext::WAITING);
        }else ctx.setStatus(NodeContext::FALSE);
        printStatus(ctx.getStatus(),ctx.currentLevel);
        return;
      }

      if(this->range.length()>this->getTerm()->range.max()){
        ctx.setStatus(NodeContext::FALSE);
        ctx.termIndex = this->range.min();
        if(this->areWeCurrent(ctx))this->parentMatch(ctx);
        printStatus(ctx.getStatus(),ctx.currentLevel);
        return;
      }

      if(ctx.calledFromChildOrRecheck){
        ctx.setStatus(NodeContext::FALSE);
        ctx.termIndex = this->range.min();
        this->parentMatch(ctx);
        printStatus(ctx.getStatus(),ctx.currentLevel);
        return;
      }

      ctx.termIndex = this->range.min();
      if(ctx.getTerm()!=this->getTerm()->type){
        ctx.setStatus(NodeContext::FALSE);
      }else{
        ctx.next();
        ctx.setStatus(NodeContext::TRUE);
      }

      if(this->areWeCurrent(ctx))
        this->parentMatch(ctx);

      printStatus(ctx.getStatus(),ctx.currentLevel);
    }

    std::string TermNode::str()const{
      std::stringstream ss;
      ss<<this->getTerm()->str();
      return ss.str();
    }

  }
}
