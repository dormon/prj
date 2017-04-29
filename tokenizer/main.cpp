#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include<geCore/lexer.h>


int main(){
  ge::core::Lexer a;
  do{
    if(std::cin.eof())break;
    std::cout<<">>> ";
    std::string line;
    std::getline(std::cin,line);
    if(line == "quit")break;
    a.parse(line);
    do{
      auto t=a.get();
      if(t.getType()==ge::core::Token::WAITING)
        break;
      std::cout<<t.str()<<std::endl;
    }while(true);
  }while(true);
  a.parse("",true);
  std::cout<<std::endl;

  //<BODY>      ::= <STATEMENT>
  //<BODY>      ::= <STATEMENT> <BODY>
  //
  //<STATEMENT> ::= <TYPE> <IDENTIFIER> ;
  //<STATEMENT> ::= <TYPE> <IDENTIFIER> ( <COMMA-DECL-LIST> ) { <BODY> }
  //<STATEMENT> ::= <IDENTIFIER> = <IDENTIFIER> ( <COMMA-LIST> ) ;
  //<STATEMENT> ::= <IDENTIFIER> <ASSIGN-OPER> <IDENTIFIER> ;
  //<STATEMENT> ::= <IDENTIFIER> <ASSIGN-OPER> <IDENTIFIER> <OPERATOR> <IDENTIFIER> ;
  //<STATEMENT> ::= return <IDENTIFIER> ;
  //<STATEMENT> ::= if ( <IDENTIFIER> ) <STATEMENT>
  //<STATEMENT> ::= if ( <IDENTIFIER> ) <STATEMENT> else <STATEMENT>
  //<STATEMENT> ::= while ( <IDENTIFIER> ) <STATEMENT>
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
  //
  //TODO a.x
  //TODO a[b]
  //TODO TYPE
  //TODO OPER
  //TODO ASSIGN-OPER

  //this language should be low level and verbose
  //so this:
  //
  //int a=32;
  //if(a<100)
  //  a+=128;
  //
  //should be expanded into
  //
  //i32 a;
  //i32 _c0;
  //i32 _c1;
  //bool cond;
  //a = 32;
  //_c0 = 100;
  //_c1 = 128;
  //cond = a<_c;
  //if(cond) a+=c1;
  //
  //
  //
  //there are commands for interpret:
  //declaration -> create resource
  //function definition -> create new function and register it
  //typedef -> register new type using TypeRegister
  //
  //
  //
  //<BODY> ::= <STATEMENT>
  //<BODY> ::= <STATEMENT> <SCRIPT>
  //
  //<STATEMENT> ::= { <BODY> }
  //<STATEMENT> ::= if ( <EXPRESSION> ) <STATEMENT>
  //<STATEMENT> ::= if ( <EXPRESSION> ) { <STATEMENT> } else { <STATEMENT> }
  //<STATEMENT> ::= while ( <EXPRESSION> ) <STATEMENT>
  //<STATEMENT> ::= <EXPRESSION> ;
  //
  //<COMMAND> ::= typedef struct { <DECL-LIST> } <IDENTIFIER> ;
  //<COMMAND> ::= typedef <TYPE> [ <INTEGER> ] <IDENTIFIER> ;
  //<COMMAND> ::= <TYPE> <IDENTIFIER> ;
  //
  //<DECL-LIST> ::= <TYPE> <IDENTIFIER> ;
  //<DECL-LIST> ::= <TYPE> <IDENTIFIER> ; <DECL-LIST>
  //



  return 0;
}





