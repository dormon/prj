#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<geCore/registerStdFunctions.h>
#include<geCore/producer.h>
#include<geCore/syntax.h>

#include<geCore/lexer.h>
#include<geUtil/NamespaceWithUsers.h>
#include<limits>

/* 
 *
 * Type = {std::size_t,Name}
 * TypeInstance = {Type,Data}
 * NamedTypeInstance = {Type,Data,InstanceName}
 *
 * FunctionType = Type
 * FunctionDefinition = {FunctionType,Definition,DefinitionName}
 * FunctionInstance = {FunctionDefinition}
 * NamedFunctionInstance = {FunctionDefinition,InstanceName}
 *
 *
 * Verbose Style:
 * Accessor += InstanceName
 * Function += InstanceName
 *
 *
 */

template<typename VAR_TYPE,typename VAL_TYPE>
void assignValueIntoVariable(std::shared_ptr<ge::core::sim::Namespace>const&nm,std::string var,VAL_TYPE val){
  if(val>std::numeric_limits<VAR_TYPE>::max()||val<std::numeric_limits<VAR_TYPE>::min())
    std::cerr<<"WARNING: overflow in assignment of value "<<val<<" into variable "<<var<<std::endl;
  nm->get<VAR_TYPE>(var)=(VAR_TYPE)val;
}

int main(){
  

  //TODO +,* is commutative so it can be right to left associative 
  //and it can be faster to analyze
  //
  //TODO what other operators can be right to left associative?
  //
  //aa=b=c=d <=> a=(b=(c=d))
  //
  //TODO lvalue for assignment operators
  //TODO optional initialization of variables
  //TODO access members of arrays and structures

  //
  //<EXPRESSION> ::= <EXP-OR>
  //<EXPRESSION> ::= <EXPRESSION> <ASSIGN-OPER> <EXP-OR>
  //
  //<EXP-OR>   ::= <EXP-AND>
  //<EXP-OR>   ::= <EXP-AND> || <EXP-OR>
  //
  //<EXP-AND>  ::= <EXP-BOR>
  //<EXP-AND>  ::= <EXP-BOR> && <EXP-AND>
  //
  //<EXP-BOR>  ::= <EXP-XOR>
  //<EXP-BOR>  ::= <EXP-XOR> | <EXP-BOR>
  //
  //<EXP-XOR>  ::= <EXP-BAND>
  //<EXP-XOR>  ::= <EXP-BAND> ^ <EXP-XOR>
  //
  //<EXP-BAND> ::= <EXP-EQUALITY>
  //<EXP-BAND> ::= <EXP-EQUALITY> & <EXP-BAND>
  //
  //<EXP-EQUALITY> ::= <EXP-RELATIONAL>
  //<EXP-EQUALITY> ::= <EXP-RELATIONAL> == <EXP-EQUALITY>
  //<EXP-EQUALITY> ::= <EXP-RELATIONAL> != <EXP-EQUALITY>
  //
  //<EXP-RELATIONAL> ::= <EXP-SHIFT>
  //<EXP-RELATIONAL> ::= <EXP-SHIFT> <  <EXP-RELATIONAL>
  //<EXP-RELATIONAL> ::= <EXP-SHIFT> >  <EXP-RELATIONAL>
  //<EXP-RELATIONAL> ::= <EXP-SHIFT> <= <EXP-RELATIONAL>
  //<EXP-RELATIONAL> ::= <EXP-SHIFT> >= <EXP-RELATIONAL>
  //
  //<EXP-SHIFT> ::= <EXP-ADDITIVE>
  //<EXP-SHIFT> ::= <EXP-ADDITIVE> << <EXP-SHIFT>
  //<EXP-SHIFT> ::= <EXP-ADDITIVE> >> <EXP-SHIFT>
  //
  //<EXP-ADDITIVE> ::= <EXP-MULTIPLICATIVE>
  //<EXP-ADDITIVE> ::= <EXP-MULTIPLICATIVE> + <EXP-ADDITIVE>
  //<EXP-ADDITIVE> ::= <EXP-MULTIPLICATIVE> - <EXP-ADDITIVE>
  //
  //<EXP-MULTIPLICATIVE> ::= <EXP-UNARY>
  //<EXP-MULTIPLICATIVE> ::= <EXP-UNARY> * <EXP-MULTIPLICATIVE>
  //<EXP-MULTIPLICATIVE> ::= <EXP-UNARY> / <EXP-MULTIPLICATIVE>
  //<EXP-MULTIPLICATIVE> ::= <EXP-UNARY> % <EXP-MULTIPLICATIVE>
  //
  //<EXP-UNARY> ::= <EXP-TERM>
  //<EXP-UNARY> ::= + <EXP-TERM>
  //<EXP-UNARY> ::= - <EXP-TERM>
  //<EXP-UNARY> ::= ~ <EXP-TERM>
  //<EXP-UNARY> ::= ! <EXP-TERM>
  //
  //<EXP-TERM> ::= ( <EXPRESSION> )
  //<EXP-TERM> ::= <IDENTIFIER>
  //<EXP-TERM> ::= <VALUE>
  //<EXP-TERM> ::= <IDENTIFIER> ( <COMMA-EXP-LIST> )
  //
  //<COMMA-EXP-LIST> ::= <EXPRESSION>
  //<COMMA-EXP-LIST> ::= <EXPRESSION> , <COMMA-EXP-LIST>
  //
  //<BODY>      ::= <STATEMENT>
  //<BODY>      ::= <STATEMENT> <BODY>
  //
  //<STATEMENT> ::= <TYPE> <IDENTIFIER> ;
  //<STATEMENT> ::= <TYPE> <IDENTIFIER> ( <COMMA-DECL-LIST> ) { <BODY> }
  //<STATEMENT> ::= return <EXPRESSION> ;
  //<STATEMENT> ::= <EXPRESSION> ;
  //
  //<STATEMENT> ::= return <EXPRESSION> ;
  //<STATEMENT> ::= if ( <EXPRESSION> ) <STATEMENT> else <STATEMENT>
  //<STATEMENT> ::= if ( <EXPRESSION> ) <STATEMENT>
  //<STATEMENT> ::= while ( <EXPRESSION> ) <STATEMENT>
  //<STATEMENT> ::= { <BODY> }
  //<STATEMENT> ::= typedef struct { <DECL-LIST> } <IDENTIFIER> ;
  //<STATEMENT> ::= typedef <TYPE> [ <INTEGER> ] <IDENTIFIER> ;
  //
  //<DECL-LIST> ::= <TYPE> <IDENTIFIER> ;
  //<DECL-LIST> ::= <TYPE> <IDENTIFIER> ; <DECL-LIST>
  //
  //<COMMA-DECL-LIST> ::= <TYPE> <IDENTIFIER>
  //<COMMA-DECL-LIST> ::= <TYPE> <IDENTIFIER> , <COMMA-DECL-LIST>
  //
  //<COMMA-LIST> ::= <IDENTIFIER>
  //<COMMA-LIST> ::= <IDENTIFIER> , <COMMA-LIST>


  struct ParserData{
    std::shared_ptr<ge::core::TypeRegister>typeRegister = std::make_shared<ge::core::TypeRegister>();
    std::shared_ptr<ge::core::sim::Namespace>sData = std::make_shared<ge::core::sim::Namespace>("*");
    ge::core::TypeRegister::TypeID currentType = ge::core::TypeRegister::UNREGISTERED;
    std::vector<ge::core::Token>tokens;
//    using TokenIndex = decltype(tokens)::size_type;

    static void addVariable(ge::core::SyntaxNode::Node node,void*data){
      std::cout<<"jojo"<<std::endl;
      ParserData*pd=(ParserData*)data;
      if(pd->currentType==ge::core::TypeRegister::UNREGISTERED){
        std::cerr<<"ERROR: addVariable() - wrong type"<<std::endl;
        return;
      }
      auto n=std::dynamic_pointer_cast<ge::core::NontermNode>(node);
      auto name = pd->tokens[n->childs[1]->range.min()].getRawData();
      auto var = pd->typeRegister->sharedAccessor(pd->currentType);
      pd->sData->insert(name,var);
    }
    static void getType(ge::core::SyntaxNode::Node node,void*data){
      ParserData*pd=(ParserData*)data;
      pd->currentType = pd->typeRegister->getTypeId(pd->tokens[node->range.min()].getRawData().c_str());
    }

    static void assignValue(ge::core::SyntaxNode::Node node,void*data){
      ParserData*pd=(ParserData*)data;
      auto varName = pd->tokens[node->range.min()].getRawData();
      auto valueToken = pd->tokens[node->range.min()+2];
      if(!pd->sData->contain(varName)){
        std::cerr<<"ERROR: assignValue() cannot assign value into non existing variable "<<varName<<std::endl;
        return;
      }
      auto varType = pd->sData->getVariable(varName)->getId();
      switch(valueToken.getType()){
        case ge::core::Token::STRING:
          if(!pd->typeRegister->stringType(varType)){
            std::cerr<<"ERROR: assignValue() cannot assign value "<<"\""<<valueToken.getRawData()<<"\""<<" into variable "<<varName<<" - different type"<<std::endl;
            return;
          }
          pd->sData->get<std::string>(varName)=valueToken.getStringData();
          break;
        case ge::core::Token::INTEGER:
          if(!pd->typeRegister->integerType(varType)){
            std::cerr<<"ERROR: assignValue() cannot assign value "<<"\""<<valueToken.getRawData()<<"\""<<" into variable "<<varName<<" - different type"<<std::endl;
            return;
          }
          switch(pd->typeRegister->getTypeDescriptionElem(varType,0)){
            case ge::core::TypeRegister::I8:
              assignValueIntoVariable<char              >(pd->sData,varName,valueToken.getIntegerData());
              break;
            case ge::core::TypeRegister::I16:
              assignValueIntoVariable<short             >(pd->sData,varName,valueToken.getIntegerData());
              break;
            case ge::core::TypeRegister::I32:
              assignValueIntoVariable<int               >(pd->sData,varName,valueToken.getIntegerData());
              break;
            case ge::core::TypeRegister::I64:
              assignValueIntoVariable<long long         >(pd->sData,varName,valueToken.getIntegerData());
              break;
            case ge::core::TypeRegister::U8:
              assignValueIntoVariable<unsigned char     >(pd->sData,varName,valueToken.getIntegerData());
              break;
            case ge::core::TypeRegister::U16:
              assignValueIntoVariable<unsigned short    >(pd->sData,varName,valueToken.getIntegerData());
              break;
            case ge::core::TypeRegister::U32:
              assignValueIntoVariable<unsigned          >(pd->sData,varName,valueToken.getIntegerData());
              break;
            case ge::core::TypeRegister::U64:
              assignValueIntoVariable<unsigned long long>(pd->sData,varName,valueToken.getIntegerData());
              break;
            default:
              break;
          }
          break;
        case ge::core::Token::FLOAT:
          if(!pd->typeRegister->floatType(varType)){
            std::cerr<<"ERROR: assignValue() cannot assign value "<<"\""<<valueToken.getRawData()<<"\""<<" into variable "<<varName<<" - different type"<<std::endl;
            return;
          }
          switch(pd->typeRegister->getTypeDescriptionElem(varType,0)){
            case ge::core::TypeRegister::F32:
              assignValueIntoVariable<float>(pd->sData,varName,valueToken.getDoubleData());
              break;
            case ge::core::TypeRegister::F64:
              assignValueIntoVariable<float>(pd->sData,varName,valueToken.getDoubleData());
              break;
            default:
              break;
          }
          break;
        case ge::core::Token::CHAR:
          pd->sData->get<char>(varName)=(char)valueToken.getIntegerData();
          break;
        default:
          break;
      }

    }
  };

  static ParserData pData;


  ge::core::Syntax syn("STATEMENT");
  for(ge::core::Token::Type i=ge::core::Token::LEFT_BRACKET;i<=ge::core::Token::KEY_RETURN;i=(ge::core::Token::Type)((unsigned)i+1)){
    std::stringstream ss;
    ss<<i;
    syn.registerTermName(i,ss.str());
  }

  //*
  syn.addRule("STATEMENT","EXPRESSION",ge::core::Token::SEMICOLON);

#if 1
  syn.addRule("EXPRESSION",
      "EXP-OR");
  syn.addRule("EXPRESSION",
      "EXPRESSION","ASSIGN-OPER",
      "EXP-OR");
#else
  syn.addRule("EXPRESSION",
      "EXP-TERM");
  syn.addRule("EXPRESSION",
      "EXPRESSION","ASSIGN-OPER",
      "EXP-TERM");
#endif


#if 1
  syn.addRule("EXP-OR",
      "EXP-AND");
  syn.addRule("EXP-OR",
      "EXP-AND",
      ge::core::Token::OR,
      "EXP-OR");
#else
  syn.addRule("EXP-OR",
      "EXP-TERM");
  syn.addRule("EXP-OR",
      "EXP-TERM",
      ge::core::Token::OR,
      "EXP-OR");
#endif

#if 1
  syn.addRule("EXP-AND",
      "EXP-BOR");
  syn.addRule("EXP-AND",
      "EXP-BOR",
      ge::core::Token::AND,
      "EXP-AND");
#else
  syn.addRule("EXP-AND",
      "EXP-TERM");
  syn.addRule("EXP-AND",
      "EXP-TERM",
      ge::core::Token::AND,
      "EXP-AND");
#endif


#if 1
  syn.addRule("EXP-BOR",
      "EXP-XOR");
  syn.addRule("EXP-BOR",
      "EXP-XOR",
      ge::core::Token::BINARY_OR,
      "EXP-BOR");
#else
  syn.addRule("EXP-BOR",
      "EXP-TERM");
  syn.addRule("EXP-BOR",
      "EXP-TERM",
      ge::core::Token::BINARY_OR,
      "EXP-BOR");
#endif


#if 1
  syn.addRule("EXP-XOR",
      "EXP-BAND");
  syn.addRule("EXP-XOR",
      "EXP-BAND",
      ge::core::Token::XOR,
      "EXP-XOR");
#else
  syn.addRule("EXP-XOR",
      "EXP-TERM");
  syn.addRule("EXP-XOR",
      "EXP-TERM",
      ge::core::Token::XOR,
      "EXP-XOR");
#endif


#if 1
  syn.addRule("EXP-BAND",
      "EXP-EQUALITY");
  syn.addRule("EXP-BAND",
      "EXP-EQUALITY",
      ge::core::Token::BINARY_AND,
      "EXP-BAND");
#else
  syn.addRule("EXP-BAND",
      "EXP-TERM");
  syn.addRule("EXP-BAND",
      "EXP-TERM",
      ge::core::Token::BINARY_OR,
      "EXP-BAND");
#endif

#if 1
  syn.addRule("EXP-EQUALITY",
      "EXP-RELATIONAL");
  syn.addRule("EXP-EQUALITY",
      "EXP-RELATIONAL",
      "EQUALITY-OPER",
      "EXP-EQUALITY");
#else
  syn.addRule("EXP-EQUALITY",
      "EXP-TERM");
  syn.addRule("EXP-EQUALITY",
      "EXP-TERM",
      "EQUALITY-OPER",
      "EXP-EQUALITY");
#endif


#if 1
  syn.addRule("EXP-RELATIONAL",
      "EXP-SHIFT");
  syn.addRule("EXP-RELATIONAL",
      "EXP-SHIFT",
      "RELATIONAL-OPER",
      "EXP-RELATIONAL");
#else
  syn.addRule("EXP-RELATIONAL",
      "EXP-TERM");
  syn.addRule("EXP-RELATIONAL",
      "EXP-TERM",
      "RELATIONAL-OPER",
      "EXP-RELATIONAL");
#endif


#if 1
  syn.addRule("EXP-SHIFT",
      "EXP-ADDITIVE");
  syn.addRule("EXP-SHIFT",
      "EXP-ADDITIVE",
      "SHIFT-OPER",
      "EXP-SHIFT");
#else
  syn.addRule("EXP-SHIFT",
      "EXP-TERM");
  syn.addRule("EXP-SHIFT",
      "EXP-TERM",
      "SHIFT-OPER",
      "EXP-SHIFT");
#endif


#if 1
  syn.addRule("EXP-ADDITIVE",
      "EXP-MULTIPLICATIVE");
  syn.addRule("EXP-ADDITIVE",
      "EXP-MULTIPLICATIVE",
      "ADDITIVE-OPER",
      "EXP-ADDITIVE");
#else
  syn.addRule("EXP-ADDITIVE",
      "EXP-TERM");
  syn.addRule("EXP-ADDITIVE",
      "EXP-TERM",
      "ADDITIVE-OPER",
      "EXP-ADDITIVE");
#endif


#if 1
  syn.addRule("EXP-MULTIPLICATIVE",
      "EXP-UNARY");
  syn.addRule("EXP-MULTIPLICATIVE",
      "EXP-UNARY",
      "MULTIPLICATIVE-OPER",
      "EXP-MULTIPLICATIVE");
#else
  syn.addRule("EXP-MULTIPLICATIVE",
      "EXP-TERM");
  syn.addRule("EXP-MULTIPLICATIVE",
      "EXP-TERM",
      "MULTIPLICATIVE-OPER",
      "EXP-MULTIPLICATIVE");
#endif


  syn.addRule("EXP-UNARY",
      "EXP-TERM");
  syn.addRule("EXP-UNARY",
      "UNARY-OPER",
      "EXP-TERM");


  syn.addRule("EXP-TERM",
      ge::core::Token::LEFT_BRACKET,
      "EXPRESSION",
      ge::core::Token::RIGHT_BRACKET);
  syn.addRule("EXP-TERM",
      ge::core::Token::IDENTIFIER);
  syn.addRule("EXP-TERM",
      "VALUE");
  syn.addRule("EXP-TERM",
      ge::core::Token::IDENTIFIER,
      ge::core::Token::LEFT_BRACKET,
      "COMMA-EXP-LIST",
      ge::core::Token::RIGHT_BRACKET);

  syn.addRule("COMMA-EXP-LIST",
      "EXPRESSION");
  syn.addRule("COMMA-EXP-LIST",
      "EXPRESSION",
      ge::core::Token::COMMA,
      "COMMA-EXP-LIST");

  syn.addRule("ASSIGN-OPER",ge::core::Token::ASSIGNMENT               );
  syn.addRule("ASSIGN-OPER",ge::core::Token::PLUS_ASSIGNMENT          );
  syn.addRule("ASSIGN-OPER",ge::core::Token::MINUS_ASSIGNMENT         );
  syn.addRule("ASSIGN-OPER",ge::core::Token::MULTIPLICATION_ASSIGNMENT);
  syn.addRule("ASSIGN-OPER",ge::core::Token::DIVISION_ASSIGNMENT      );
  syn.addRule("ASSIGN-OPER",ge::core::Token::MODULO_ASSIGNMENT        );
  syn.addRule("ASSIGN-OPER",ge::core::Token::BINARY_AND_ASSIGNMENT    );
  syn.addRule("ASSIGN-OPER",ge::core::Token::BINARY_OR_ASSIGNMENT     );
  syn.addRule("ASSIGN-OPER",ge::core::Token::LEFT_SHIFT_ASSIGNMENT    );
  syn.addRule("ASSIGN-OPER",ge::core::Token::RIGHT_SHIFT_ASSIGNMENT   );

  syn.addRule("EQUALITY-OPER",ge::core::Token::EQUAL    );
  syn.addRule("EQUALITY-OPER",ge::core::Token::NOT_EQUAL);

  syn.addRule("RELATIONAL-OPER",ge::core::Token::LESS         );
  syn.addRule("RELATIONAL-OPER",ge::core::Token::GREATER      );
  syn.addRule("RELATIONAL-OPER",ge::core::Token::LESS_EQUAL   );
  syn.addRule("RELATIONAL-OPER",ge::core::Token::GREATER_EQUAL);

  syn.addRule("SHIFT-OPER",ge::core::Token::LEFT_SHIFT   );
  syn.addRule("SHIFT-OPER",ge::core::Token::RIGHT_SHIFT  );

  syn.addRule("ADDITIVE-OPER",ge::core::Token::PLUS );
  syn.addRule("ADDITIVE-OPER",ge::core::Token::MINUS);

  syn.addRule("MULTIPLICATIVE-OPER",ge::core::Token::MULTIPLICATION);
  syn.addRule("MULTIPLICATIVE-OPER",ge::core::Token::DIVISION      );
  syn.addRule("MULTIPLICATIVE-OPER",ge::core::Token::MODULO        );

  syn.addRule("UNARY-OPER",ge::core::Token::PLUS    );
  syn.addRule("UNARY-OPER",ge::core::Token::MINUS   );
  syn.addRule("UNARY-OPER",ge::core::Token::NEGATION);
  syn.addRule("UNARY-OPER",ge::core::Token::NOT     );


  syn.addRule("VALUE",ge::core::Token::FLOAT  );
  syn.addRule("VALUE",ge::core::Token::INTEGER);
  syn.addRule("VALUE",ge::core::Token::STRING );



  // */



  /*
  //RIGHT-LEFT LEFT_RIGHT
  syn.addRule("STATEMENT","ASSIGN",ge::core::Token::SEMICOLON);
  syn.addRule("ASSIGN","ADD");
  syn.addRule("ASSIGN","ASSIGN",ge::core::Token::ASSIGNMENT,"ADD");
  syn.addRule("ADD","TERM");
  syn.addRule("ADD","TERM",ge::core::Token::PLUS,"ADD");
  syn.addRule("TERM",ge::core::Token::IDENTIFIER);
  syn.addRule("TERM",ge::core::Token::LEFT_BRACKET,"ADD",ge::core::Token::RIGHT_BRACKET);
  // */


  /*
  //LEFT-RIGHT LEFT-RIGHT
  syn.addRule("STATEMENT","ADD",ge::core::Token::SEMICOLON);
  syn.addRule("ADD","MULT");
  syn.addRule("ADD","MULT",ge::core::Token::PLUS,"ADD");
  syn.addRule("MULT","TERM");
  syn.addRule("MULT","TERM",ge::core::Token::MULTIPLICATION,"MULT");
  syn.addRule("TERM",ge::core::Token::IDENTIFIER);
  syn.addRule("TERM",ge::core::Token::LEFT_BRACKET,"ADD",ge::core::Token::RIGHT_BRACKET);
  // */

  /*
     syn.addRule("STATEMENT","A",ge::core::Token::SEMICOLON);
     syn.addRule("A","A",ge::core::Token::DOT);
     syn.addRule("A",ge::core::Token::DOT);
  // */

  /*
     syn.addRule("EXPRESSION",
     "EXP-OR");
     syn.addRule("EXPRESSION",
     "EXPRESSION","ASSIGN-OPER","EXP-OR");

     syn.addRule("EXP-OR",
     "EXP-AND");
     syn.addRule("EXP-OR",
     "EXP-AND",
     ge::core::Token::OR,
     "EXP-OR");

     syn.addRule("EXP-AND",
     "EXP-BOR");
     syn.addRule("EXP-AND",
     "EXP-BOR",
     ge::core::Token::AND,
     "EXP-AND");

     syn.addRule("EXP-BOR",
     "EXP-XOR");
     syn.addRule("EXP-BOR",
     "EXP-XOR",
     ge::core::Token::BINARY_OR,
     "EXP-BOR");

     syn.addRule("EXP-XOR",
     "EXP-BAND");
     syn.addRule("EXP-XOR",
     "EXP-BAND",
     ge::core::Token::XOR,
     "EXP-XOR");

     syn.addRule("EXP-BAND",
     "EXP-EQUALITY");
     syn.addRule("EXP-BAND",
     "EXP-EQUALITY",
     ge::core::Token::BINARY_OR,
     "EXP-BAND");

     syn.addRule("EXP-EQUALITY",
     "EXP-RELATIONAL");
     syn.addRule("EXP-EQUALITY",
     "EXP-RELATIONAL",
     ge::core::Token::EQUAL,
     "EXP-EQUALITY");
     syn.addRule("EXP-EQUALITY",
     "EXP-RELATIONAL",
     ge::core::Token::NOT_EQAUL,
     "EXP-EQUALITY");

     syn.addRule("EXP-RELATIONAL",
     "EXP-SHIFT");
     syn.addRule("EXP-RELATIONAL",
     "EXP-SHIFT",
     ge::core::Token::LESS,
     "EXP-RELATIONAL");
     syn.addRule("EXP-RELATIONAL",
     "EXP-SHIFT",
     ge::core::Token::GREATER,
     "EXP-RELATIONAL");
     syn.addRule("EXP-RELATIONAL",
     "EXP-SHIFT",
     ge::core::Token::LESS_EQUAL,
     "EXP-RELATIONAL");
     syn.addRule("EXP-RELATIONAL",
     "EXP-SHIFT",
     ge::core::Token::GREATER_EQUAL,
     "EXP-RELATIONAL");

  syn.addRule("EXP-SHIFT",
      "EXP-ADDITIVE");
  syn.addRule("EXP-SHIFT",
      "EXP-ADDITIVE",
      ge::core::Token::LEFT_SHIFT,
      "EXP-SHIFT");
  syn.addRule("EXP-SHIFT",
      "EXP-ADDITIVE",
      ge::core::Token::RIGHT_SHIFT,
      "EXP-SHIFT");

  syn.addRule("EXP-ADDITIVE",
      "EXP-MULTIPLICATIVE");
  syn.addRule("EXP-ADDITIVE",
      "EXP-MULTIPLICATIVE",
      ge::core::Token::PLUS,
      "EXP-ADDITIVE");
  syn.addRule("EXP-ADDITIVE",
      "EXP-MULTIPLICATIVE",
      ge::core::Token::MINUS,
      "EXP-ADDITIVE");

  syn.addRule("EXP-MULTIPLICATIVE",
      "EXP-UNARY");
  syn.addRule("EXP-MULTIPLICATIVE",
      "EXP-UNARY",
      ge::core::Token::MULTIPLICATION,
      "EXP-MULTIPLICATIVE");
  syn.addRule("EXP-MULTIPLICATIVE",
      "EXP-UNARY",
      ge::core::Token::DIVISION,
      "EXP-MULTIPLICATIVE");
  syn.addRule("EXP-MULTIPLICATIVE",
      "EXP-UNARY",
      ge::core::Token::MODULO,
      "EXP-MULTIPLICATIVE");

  syn.addRule("EXP-UNARY",
      "EXP-TERM");
  syn.addRule("EXP-UNARY",
      ge::core::Token::PLUS,
      "EXP-TERM");
  syn.addRule("EXP-UNARY",
      ge::core::Token::MINUS,
      "EXP-TERM");
  syn.addRule("EXP-UNARY",
      ge::core::Token::NEGATION,
      "EXP-TERM");
  syn.addRule("EXP-UNARY",
      ge::core::Token::NOT,
      "EXP-TERM");

  syn.addRule("EXP-TERM",
      ge::core::Token::LEFT_BRACKET,
      "EXPRESSION",
      ge::core::Token::RIGHT_BRACKET);
  syn.addRule("EXP-TERM",
      ge::core::Token::IDENTIFIER);
  syn.addRule("EXP-TERM",
      "VALUE");
  syn.addRule("EXP-TERM",
      ge::core::Token::IDENTIFIER,
      ge::core::Token::LEFT_BRACKET,
      "COMMA-EXP-LIST",
      ge::core::Token::RIGHT_BRACKET);

  syn.addRule("COMMA-EXP-LIST",
      "EXPRESSION");
  syn.addRule("COMMA-EXP-LIST",
      "EXPRESSION",
      ge::core::Token::COMMA,
      "COMMA-EXP-LIST");



  syn.addRule("BODY",
      "STATEMENT");
  syn.addRule("BODY",
      "STATEMENT",
      "BODY");

  syn.addRule("STATEMENT",
      "TYPE",
      ge::core::Token::IDENTIFIER,
      ge::core::Token::SEMICOLON,
      nullptr,pData.addVariable,&pData);

  syn.addRule("STATEMENT",
      "TYPE",
      ge::core::Token::IDENTIFIER,
      ge::core::Token::LEFT_BRACKET,
      "COMMA-DECL-LIST",
      ge::core::Token::RIGHT_BRACKET,
      ge::core::Token::LEFT_SET_BRACKET,
      "BODY",
      ge::core::Token::RIGHT_SET_BRACKET);

  syn.addRule("STATEMENT",
      ge::core::Token::KEY_RETURN,
      "EXPRESSION",
      ge::core::Token::SEMICOLON);

  syn.addRule("STATEMENT",
      "EXPRESSION",
      ge::core::Token::SEMICOLON);

  syn.addRule("STATEMENT",
      ge::core::Token::KEY_IF,
      ge::core::Token::LEFT_BRACKET,
      "EXPRESSION",
      ge::core::Token::RIGHT_BRACKET,
      "STATEMENT",
      ge::core::Token::KEY_ELSE,
      "STATEMENT");

  syn.addRule("STATEMENT",
      ge::core::Token::KEY_IF,
      ge::core::Token::LEFT_BRACKET,
      "EXPRESSION",
      ge::core::Token::RIGHT_BRACKET,
      "STATEMENT");

  syn.addRule("STATEMENT",
      ge::core::Token::KEY_WHILE,
      ge::core::Token::LEFT_BRACKET,
      "EXPRESSION",
      ge::core::Token::RIGHT_BRACKET,
      "STATEMENT");

  syn.addRule("STATEMENT",
      ge::core::Token::LEFT_SET_BRACKET,
      "BODY",
      ge::core::Token::RIGHT_SET_BRACKET);

  syn.addRule("STATEMENT",
      ge::core::Token::KEY_TYPEDEF,
      ge::core::Token::KEY_STRUCT,
      ge::core::Token::LEFT_SET_BRACKET,
      "DECL-LIST",
      ge::core::Token::RIGHT_SET_BRACKET,
      ge::core::Token::IDENTIFIER,
      ge::core::Token::SEMICOLON);

  syn.addRule("STATEMENT",
      ge::core::Token::KEY_TYPEDEF,
      "TYPE",
      ge::core::Token::LEFT_INDEX_BRACKET,
      ge::core::Token::INTEGER,
      ge::core::Token::RIGHT_INDEX_BRACKET,
      ge::core::Token::IDENTIFIER,
      ge::core::Token::SEMICOLON);

  syn.addRule("DECL-LIST",
      "TYPE",
      ge::core::Token::IDENTIFIER,
      ge::core::Token::SEMICOLON);
  syn.addRule("DECL-LIST",
      "TYPE",
      ge::core::Token::IDENTIFIER,
      ge::core::Token::SEMICOLON,
      "DECL-LIST");

  syn.addRule("COMMA-DECL-LIST",
      "TYPE",
      ge::core::Token::IDENTIFIER);


  syn.addRule("COMMA-DECL-LIST",
      "TYPE",
      ge::core::Token::IDENTIFIER,
      ge::core::Token::COMMA,
      "COMMA-DECL-LIST");

  syn.addRule("COMMA-LIST",
      ge::core::Token::IDENTIFIER);

  syn.addRule("COMMA-LIST",
      ge::core::Token::IDENTIFIER,
      ge::core::Token::COMMA,
      "COMMA-LIST");

  syn.addRule("VALUE",ge::core::Token::FLOAT  );
  syn.addRule("VALUE",ge::core::Token::INTEGER);
  syn.addRule("VALUE",ge::core::Token::STRING );

  syn.addRule("TYPE",ge::core::Token::KEY_BOOL  ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_I8    ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_I16   ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_I32   ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_I64   ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_U8    ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_U16   ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_U32   ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_U64   ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_F32   ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::KEY_F64   ,nullptr,pData.getType,&pData);
  syn.addRule("TYPE",ge::core::Token::IDENTIFIER,nullptr,pData.getType,&pData);

  syn.addRule("ASSIGN-OPER",ge::core::Token::ASSIGNMENT               );
  syn.addRule("ASSIGN-OPER",ge::core::Token::PLUS_ASSIGNMENT          );
  syn.addRule("ASSIGN-OPER",ge::core::Token::MINUS_ASSIGNMENT         );
  syn.addRule("ASSIGN-OPER",ge::core::Token::MULTIPLICATION_ASSIGNMENT);
  syn.addRule("ASSIGN-OPER",ge::core::Token::DIVISION_ASSIGNMENT      );
  syn.addRule("ASSIGN-OPER",ge::core::Token::MODULO_ASSIGNMENT        );
  syn.addRule("ASSIGN-OPER",ge::core::Token::BINARY_AND_ASSIGNMENT    );
  syn.addRule("ASSIGN-OPER",ge::core::Token::BINARY_OR_ASSIGNMENT     );
  syn.addRule("ASSIGN-OPER",ge::core::Token::LEFT_SHIFT_ASSIGNMENT    );
  syn.addRule("ASSIGN-OPER",ge::core::Token::RIGHT_SHIFT_ASSIGNMENT   );
  // */

  syn.computeLengths();

  std::cout<<syn.str()<<std::endl;

  syn.runStart();
  ge::core::Lexer a;
  do{
    if(std::cin.eof())break;
    std::cout<<">>> ";
    std::string line;
    std::getline(std::cin,line);
    if(line == "quit")break;
    if(line == "print"){
      for(auto x:syn.ctx.terms)
        std::cout<<syn._termNames[x]<<" ";
      std::cout<<std::endl;
      continue;
    }
    if(line == "syntax"){
      std::cout<<syn.getSyntaxTree()->str()<<std::endl;
      continue;
    }
    if(line == "flush"){
      syn.runStart();
      continue;
    }
    if(line == "namespace"){
      std::cout<<pData.sData->toStr(0)<<std::endl;
      continue;
    }
    if(line == "types"){
    }
    line+="\n";
    a.parse(line);
    do{
      auto t=a.get();
      if(t.getType()==ge::core::Token::WAITING)
        break;
      pData.tokens.push_back(t);
      syn.addTerm(t.getType());
    }while(true);
    auto status=syn.runContinue();
    if      (status == ge::core::NodeContext::TRUE){
      syn.processTree(syn.getSyntaxTree());
      pData.tokens.clear();
      std::cout<<syn.getSyntaxTree()->str()<<std::endl;
      syn.runStart();
    }else if(status == ge::core::NodeContext::FALSE){
      pData.tokens.clear();
      std::cout<<"syntax error"<<std::endl;
      syn.runStart();
    }
  }while(true);
  a.parse("",true);
  std::cout<<std::endl;


  return 0;
}
