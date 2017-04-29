#include"asmSyntax.h"

using namespace ge::core;

AsmSyntax::AsmSyntax(){
}

AsmSyntax::~AsmSyntax(){
}

void AsmSyntax::addToken(Token const&t){
  this->STATEMENT(t);
}

void AsmSyntax::STATEMENT(Token const&t){
  static enum State{
    START,
    TYPE,
    IDENTIFIER,
    RETURN,
    IF,
    WHILE,
    LSET,
    TYPEDEF,
    TYPE_IDENTIFIER,
    IDENTIFIER_ASSIGN,
    IDENTIFIER_ASSIGNOPER,
    RETURN_IDENTIFIER,
    IF_L,
    WHILE_L,
    TYPEDEF_STRUCT,
    TYPEDEF_TYPE,
    TYPE_IDENTIFIER_L,
    IDENTIFIER_ASSIGN_IDENTIFIER,
    IF_L_IDENTIFIER,

  }state = START;

  static ge::core::FSA fsa(
      "START",
      "START"     ,"T","TYPE"        ,
      "START"     ,"I","ID"          ,
      "START"     ,"R","RET"         ,
      "START"     ,"f","IF"          ,
      "START"     ,"w","WHILE"       ,
      "START"     ,"{","LSET"        ,
      "START"     ,"t","TYPEDEF"     ,
      "TYPE"      ,"I","TYPE_ID"     ,
      "ID"        ,"a","ID_A"        ,
      "ID_A"      ,"I","ID_A_ID"     ,
      "ID_A_ID"   ,"(","ID_A_ID_("   ,
      "ID"        ,"s","ID_AO"       ,
      "RET"       ,"I","RET_ID"      ,
      "IF"        ,"(","IF_L"        ,
      "WHILE"     ,"(","WHILE_L"     ,
      "WHILE_L"   ,"I","WHILE_L_ID"  ,
      "WHILE_L_ID",")","WHILE_L_ID_R",
      "WHILE_L_ID_R"



  switch(t.getType()){
    case Token::KEY_RETURN:
  }
}
