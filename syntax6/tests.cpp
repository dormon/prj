
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

  /*
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

}
