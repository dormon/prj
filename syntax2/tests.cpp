
int main(){
  //<PARAMS>        ::= "(" ")"
  //<PARAMS>        ::= "(" <FUNCTION-LIST> ")"
  //<FUNCTION>      ::= "identifier" "(" ")"
  //<FUNCTION-LIST> ::= <FUNCTION>
  //<FUNCTION-LIST> ::= <FUNCTION> "," <FUNCTION-LIST>


  enum Tokens{
    LEFT,
    RIGHT,
    COMMA,
    IDENTIFIER
  };

  ge::core::Syntax syntax("FUNCTION");
  std::vector<std::weak_ptr<ge::core::Symbol>>ss;
  syntax.addRule("PARAMS"       ,LEFT,RIGHT                      );
  syntax.addRule("PARAMS"       ,LEFT,"FUNCTION-LIST",RIGHT      );
  syntax.addRule("FUNCTION"     ,IDENTIFIER,"PARAMS"             );
  syntax.addRule("FUNCTION-LIST","FUNCTION"                      );
  syntax.addRule("FUNCTION-LIST","FUNCTION",COMMA,"FUNCTION-LIST");


  std::cout<<syntax.str()<<std::endl;

  syntax.runStart();
  syntax.addTerm(IDENTIFIER);
  syntax.addTerm(LEFT      );
  syntax.addTerm(IDENTIFIER);
  syntax.addTerm(LEFT      );
  syntax.addTerm(RIGHT     );
  syntax.addTerm(COMMA     );
  syntax.addTerm(IDENTIFIER);
  syntax.addTerm(LEFT      );
  syntax.addTerm(RIGHT     );
  syntax.addTerm(RIGHT     );
  std::cout<<syntax.runContinue()<<std::endl;
  std::cout<<syntax.getSyntaxTree()->str()<<std::endl;


}
