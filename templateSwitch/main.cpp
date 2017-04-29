#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include"syntax.h"


template<typename...ARGS>
unsigned getNextState(unsigned,unsigned){
  return-1;
}

template<typename...ARGS>
unsigned getNextState(
    unsigned const&state,
    unsigned const&lex,
    std::tuple<unsigned,unsigned,unsigned>const&tr,
    ARGS...args){
  if(std::get<0>(tr)==state && std::get<1>(tr)==lex)
    return std::get<2>(tr);
  return getNextState(state,lex,args...);
}


template<unsigned a,unsigned l,unsigned b,typename...ARGS>
void setTr(std::map<std::pair<unsigned,unsigned>,unsigned>&tr){
  tr[std::make_pair(a,l)]=b;
  setTr<ARGS...>(tr);
}

template<typename...ARGS>
unsigned gState(unsigned state,unsigned lex){
  bool nonInitialized=true;
  std::map<std::pair<unsigned,unsigned>,unsigned>tr;
  if(nonInitialized){
    setTr<ARGS...>(tr);
    nonInitialized=false;
  }
  auto ii=tr.find(std::make_pair(state,lex));
  if(ii==tr.end())return-1;
  return ii->second;
}

int main(){
  //<PARAMS>        ::= "(" ")"
  //<PARAMS>        ::= "(" <FUNCTION-LIST> ")"
  //<FUNCTION>      ::= "identifier" "(" ")"
  //<FUNCTION-LIST> ::= <FUNCTION>
  //<FUNCTION-LIST> ::= <FUNCTION> "," <FUNCTION-LIST>
  //Syntax syntax;
  //syntax.addNonterm()

  
  enum Tokens{
    LEFT,
    RIGHT,
    COMMA,
    IDENTIFIER
  };

  Syntax syntax("FUNCTION");
  syntax.addRule("PARAMS"       ,LEFT,RIGHT                      );
  syntax.addRule("PARAMS"       ,LEFT,"FUNCTION-LIST",RIGHT      );
  syntax.addRule("FUNCTION"     ,IDENTIFIER,"PARAMS"             );
  syntax.addRule("FUNCTION-LIST","FUNCTION"                      );
//  syntax.addRule("FUNCTION-LIST",IDENTIFIER,"PARAMS"             );
  syntax.addRule("FUNCTION-LIST","FUNCTION",COMMA,"FUNCTION-LIST");
  std::cout<<syntax.str()<<std::endl;
  
  syntax.addToken(
      Token(IDENTIFIER),
      Token(LEFT      ),
      Token(IDENTIFIER),
      Token(LEFT      ),
      Token(RIGHT     ),
      Token(COMMA     ),
      Token(IDENTIFIER),
      Token(LEFT      ),
      Token(RIGHT     ),
      Token(RIGHT     ));
  syntax.runStart();
  std::cout<<syntax.runContinue()<<std::endl;



  std::cout<<getNextState(0,0,
      std::make_tuple<unsigned,unsigned,unsigned>(0,1,1),
      std::make_tuple<unsigned,unsigned,unsigned>(0,0,2),
      std::make_tuple<unsigned,unsigned,unsigned>(1,0,0),
      std::make_tuple<unsigned,unsigned,unsigned>(1,1,2),
      std::make_tuple<unsigned,unsigned,unsigned>(2,0,0),
      std::make_tuple<unsigned,unsigned,unsigned>(2,1,0))<<std::endl;
  return 0;
}
