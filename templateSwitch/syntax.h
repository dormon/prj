#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<memory>
#include<sstream>
#include<cstring>

using TermType = unsigned;

struct Token{
  Token();
  Token(TermType const&type);
  TermType type;
  std::string data;
};

Token::Token(){
}

Token::Token(TermType const&type){
  this->type = type;
}

class Context;

class Symbol{
  public:
    enum Status{
      TRUE    = 0,
      WAITING = 1,
      FALSE   = 2,
    };
    virtual Status match(Context&ctx)=0;
    virtual std::string str()const = 0;
};

class Nonterm: public Symbol{
  public:
    std::vector<std::vector<std::weak_ptr<Symbol>>>rightSides;
    std::string name;
    Nonterm(std::string name);
    virtual~Nonterm();
    void addSide(std::vector<std::weak_ptr<Symbol>>side);
    virtual Status match(Context&ctx);
    virtual std::string str()const;
};

class Term: public Symbol{
  public:
    TermType term;
    inline Term(TermType const&type);
    inline virtual~Term();
    virtual Status match(Context&ctx);
    virtual std::string str()const;
};

class SyntaxNode{
  public:
    std::weak_ptr<Symbol>symbol;
    Token token;
    std::vector<std::shared_ptr<SyntaxNode>>childs;
    using SideIndex=decltype(Nonterm::rightSides)::size_type;
    SideIndex side;
    SyntaxNode(std::weak_ptr<Symbol>const&symbol,Token const&token){
      this->symbol = symbol;
      this->token = token;
    }
    SyntaxNode(std::weak_ptr<Symbol>const&symbol,SideIndex side){
      this->symbol = symbol;
      this->side = side;
    }
    bool nonterm()const{
      return std::dynamic_pointer_cast<Nonterm>(symbol.lock())!=nullptr;
    }
    void pop(){
      this->childs.pop_back();
    }
    void push(std::shared_ptr<SyntaxNode>const&node){
      this->childs.push_back(node);
    }
    void goToNextRightSide(){
      this->side++;
      this->childs.clear();
    }
};

class Context{
  public:
    std::vector<Token>tokens;
    using SideIndex = decltype(Nonterm::rightSides)::size_type;
    using ItemIndex = decltype(Nonterm::rightSides)::value_type::size_type;
    using Position  = decltype(Context::tokens)::size_type;

    Position pos = 0;
    std::vector<std::tuple<SideIndex,ItemIndex,Position>>levelCtx;
    decltype(levelCtx)::size_type level;

    inline Context();
    inline void reset();
    inline void push();
    inline void pop();
    inline void toNextLevel();
    inline void toPrevLevel();
    inline void goRight();
    inline void goToNextRightSide();
    inline bool areWeLastLevel()const;
    inline SideIndex getSideIndex()const;
    inline ItemIndex getItemIndex()const;
    inline bool end()const;
    inline Token const& getToken()const;
    inline void nextTern();
};

Nonterm::Nonterm(std::string name){
  this->name = name;
}

Nonterm::~Nonterm(){
}

void Nonterm::addSide(std::vector<std::weak_ptr<Symbol>>side){
  this->rightSides.push_back(side);
}

Symbol::Status Nonterm::match(Context&ctx){
  while(true){
    auto sideId = ctx.getSideIndex();
    auto itemId = ctx.getItemIndex();

    for(unsigned i=0;i<ctx.level;++i)
      std::cerr<<"  ";
    std::cerr<<this->name<<" "<<ctx.pos<<" sideId: "<<sideId<<", itemId: "<<itemId<<std::endl;

    if(sideId>=this->rightSides.size())break;
    if(itemId>=this->rightSides[sideId].size()){
      std::cerr<<"ERROR: right side is empty"<<std::endl;
      break;
    }

    if(ctx.areWeLastLevel())
      ctx.push();
    ctx.toNextLevel();

    auto status = this->rightSides[sideId][itemId].lock()->match(ctx);

    ctx.toPrevLevel();
    switch(status){
      case TRUE:
        ctx.pop();
        if(this->rightSides[sideId].size()==itemId+1)return TRUE;
        ctx.goRight();
        break;
      case WAITING:
        return WAITING;
        break;
      case FALSE:
        ctx.pop();
        ctx.goToNextRightSide();
        break;
    }
  }
  return FALSE;
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

Term::Term(TermType const&type){
  this->term = type;
}

Term::~Term(){
}

Symbol::Status Term::match(Context&ctx){
  if(ctx.end())return WAITING;
  if(this->term == ctx.getToken().type){
    ctx.nextTern();
    return TRUE;
  }
  return FALSE;
}

std::string Term::str()const{
  std::stringstream ss;
  ss<<"\""<<this->term<<"\"";
  return ss.str();
}


Context::Context(){
  this->reset();
}

void Context::reset(){
  this->levelCtx.clear();
  this->level = 0;
  this->pos = 0;
  this->push();
}


void Context::push(){
  this->levelCtx.push_back(std::make_tuple(0,0,this->pos));
}

void Context::pop(){
  this->levelCtx.pop_back();
}

void Context::toNextLevel(){
  this->level++;
}

void Context::toPrevLevel(){
  this->level--;
}

void Context::goRight(){
  std::get<1>(this->levelCtx[this->level])++;
}

void Context::goToNextRightSide(){
  this->pos = std::get<2>(this->levelCtx[this->level]);
  std::get<0>(this->levelCtx[this->level])++;
  std::get<1>(this->levelCtx[this->level])=0;
}

bool Context::areWeLastLevel()const{
  return this->level+1==this->levelCtx.size();
}

Context::SideIndex Context::getSideIndex()const{
  return std::get<0>(this->levelCtx[this->level]);
}

Context::ItemIndex Context::getItemIndex()const{
  return std::get<1>(this->levelCtx[this->level]);
}

bool Context::end()const{
  return this->pos>=this->tokens.size();
}

Token const& Context::getToken()const{
  return this->tokens[this->pos];
}

void Context::nextTern(){
  this->pos++;
}


class Syntax{
  public:
    Context ctx;
    std::string start;
    std::map<std::string,std::shared_ptr<Symbol>>name2Nonterm;
    std::map<TermType   ,std::shared_ptr<Symbol>>name2Term   ;
  
    Syntax(std::string start);
    ~Syntax();

    void runStart();
    template<typename...>
      void addToken();
    template<typename...ARGS>
      void addToken(Token const&token,ARGS...args);
    Symbol::Status runContinue();

    std::shared_ptr<Symbol>const&addNonterm(std::string    name);
    std::shared_ptr<Symbol>const&addTerm   (TermType const&type);

    void addRule(
        std::string nonterm,
        std::vector<std::weak_ptr<Symbol>>side);
    void addSymbol2Side(
        std::vector<std::weak_ptr<Symbol>>&side,
        std::string nonterm);
    void addSymbol2Side(
        std::vector<std::weak_ptr<Symbol>>&side,
        TermType term);

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
    void addRule(std::string nonterm,ARGS...args);

    std::string str()const;
};

Syntax::Syntax(std::string start){
  this->start = start;
}

Syntax::~Syntax(){
}

void Syntax::runStart(){
  this->ctx.reset();
}

template<typename...>
void Syntax::addToken(){
}

template<typename...ARGS>
void Syntax::addToken(Token const&token,ARGS...args){
  this->ctx.tokens.push_back(token);
  this->addToken(args...);
}

Symbol::Status Syntax::runContinue(){
  auto s=this->name2Nonterm[this->start];
  return s->match(this->ctx);
}


std::shared_ptr<Symbol>const&Syntax::addNonterm(std::string name){
  auto ii=this->name2Nonterm.find(name);
  if(ii!=this->name2Nonterm.end())
    return ii->second;
  return this->name2Nonterm[name]=std::make_shared<Nonterm>(name);
}

std::shared_ptr<Symbol>const& Syntax::addTerm(TermType const&type){
  auto ii=this->name2Term.find(type);
  if(ii!=this->name2Term.end())
    return ii->second;
  return this->name2Term[type]=std::make_shared<Term>(type);
}

void Syntax::addRule(
    std::string nonterm,
    std::vector<std::weak_ptr<Symbol>>side){
  auto n=this->addNonterm(nonterm);
  auto ptr=std::dynamic_pointer_cast<Nonterm>(n);
  ptr->addSide(side);
}

void Syntax::addSymbol2Side(
    std::vector<std::weak_ptr<Symbol>>&side,
    std::string nonterm){
  auto n=this->addNonterm(nonterm);
  side.push_back(n);
}

void Syntax::addSymbol2Side(
    std::vector<std::weak_ptr<Symbol>>&side,
    TermType term){
  auto t=this->addTerm(term);
  side.push_back(t);
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
  side.push_back(this->addNonterm(sym));
  this->addSymbol2Side(side,args...);
}

template<typename...ARGS>
void Syntax::addSymbol2Side(
    std::vector<std::weak_ptr<Symbol>>&side,
    TermType sym,
    ARGS...args){
  side.push_back(this->addTerm(sym));
  this->addSymbol2Side(side,args...);
}

template<typename...ARGS>
void Syntax::addRule(std::string nonterm,ARGS...args){
  std::vector<std::weak_ptr<Symbol>>side;
  this->addSymbol2Side(side,args...);
  auto n=this->addNonterm(nonterm);
  auto ptr=std::dynamic_pointer_cast<Nonterm>(n);
  ptr->addSide(side);
}


std::string Syntax::str()const{
  std::stringstream ss;
  for(auto x:this->name2Nonterm)
    ss<<x.second->str();
  return ss.str();
}
