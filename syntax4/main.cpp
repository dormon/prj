#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<geCore/registerStdFunctions.h>
#include<geCore/producer.h>
#include<geCore/syntax.h>
#include<geUtil/HighResolutionTimer.h>

#include<geCore/lexer.h>
#include<geUtil/NamespaceWithUsers.h>
#include<geCore/text.h>
#include<limits>

/* 
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
 */

int main(){
  //TODO what other operators can be right to left associative?
  //
  //aa=b=c=d <=> a=(b=(c=d))
  //
  //TODO lvalue for assignment operators
  //TODO optional initialization of variables
  //TODO access members of arrays and structures

  auto tokenization = std::make_shared<ge::core::Tokenization>();
  tokenization->load(ge::core::loadTextFile("/home/dormon/Desktop/lex.txt"));
  ge::core::Syntax syn(tokenization,ge::core::loadTextFile("/home/dormon/Desktop/syn.txt"));
  /*
  ge::core::Syntax syn("STATEMENT",tokenization);
  syn.addRule({"exp-assign-pass","EXPRESSION","EXP-OR"});
  syn.addRule({"exp-assign"     ,"EXPRESSION","EXPRESSION","ASSIGN-OPER","EXP-OR"});

  syn.addRule({"exp-or-pass","EXP-OR","EXP-AND"});
  syn.addRule({"exp-or"     ,"EXP-OR","EXP-AND","||","EXP-OR"});

  syn.addRule({"exp-and-pass","EXP-AND","EXP-BOR"});
  syn.addRule({"exp-and"     ,"EXP-AND","EXP-BOR","&&","EXP-AND"});

  syn.addRule({"exp-bor-pass","EXP-BOR","EXP-XOR"});
  syn.addRule({"exp-bor"     ,"EXP-BOR","EXP-XOR","|","EXP-BOR"});

  syn.addRule({"exp-xor-pass","EXP-XOR","EXP-BAND"});
  syn.addRule({"exp-xor"     ,"EXP-XOR","EXP-BAND","^","EXP-XOR"});

  syn.addRule({"exp-band-pass","EXP-BAND","EXP-EQUALITY"});
  syn.addRule({"exp-band"     ,"EXP-BAND","EXP-EQUALITY","&","EXP-BAND"});

  syn.addRule({"exp-equality-pass","EXP-EQUALITY","EXP-RELATIONAL"});
  syn.addRule({"exp-equality"     ,"EXP-EQUALITY","EXP-RELATIONAL","EQUALITY-OPER","EXP-EQUALITY"});

  syn.addRule({"exp-relational-pass","EXP-RELATIONAL","EXP-SHIFT"});
  syn.addRule({"exp-relational"     ,"EXP-RELATIONAL","EXP-SHIFT","RELATIONAL-OPER","EXP-RELATIONAL"});

  syn.addRule({"exp-shift-pass","EXP-SHIFT","EXP-ADDITIVE"});
  syn.addRule({"exp-shift"     ,"EXP-SHIFT","EXP-ADDITIVE","SHIFT-OPER","EXP-SHIFT"});

  syn.addRule({"exp-additive-pass","EXP-ADDITIVE","EXP-MULTIPLICATIVE"});
  syn.addRule({"exp-additive"     ,"EXP-ADDITIVE","EXP-MULTIPLICATIVE","ADDITIVE-OPER","EXP-ADDITIVE"});

  syn.addRule({"exp-multiplicative-pass","EXP-MULTIPLICATIVE","EXP-UNARY"});
  syn.addRule({"exp-multiplicative"     ,"EXP-MULTIPLICATIVE","EXP-UNARY","MULTIPLICATIVE-OPER","EXP-MULTIPLICATIVE"});

  syn.addRule({"exp-unary-pass","EXP-UNARY","EXP-TERM"});
  syn.addRule({"exp-unary"     ,"EXP-UNARY","UNARY-OPER","EXP-TERM"});

  syn.addRule({"exp-()"        ,"EXP-TERM","(","EXPRESSION",")"});
  syn.addRule({"exp-identifier","EXP-TERM","identifier"});
  syn.addRule({"exp-value"     ,"EXP-TERM","VALUE"});
  syn.addRule({"exp-function"  ,"EXP-TERM","identifier","(","COMMA-EXP-LIST",")"});

  syn.addRule({"comma-exp-list-term","COMMA-EXP-LIST","EXPRESSION"});
  syn.addRule({"comma-exp-list"     ,"COMMA-EXP-LIST","EXPRESSION",",","COMMA-EXP-LIST"});

  syn.addRule({"body-term","BODY","STATEMENT"});
  syn.addRule({"body"     ,"BODY","STATEMENT","BODY"});

  syn.addRule({"statement-declaration","STATEMENT","TYPE","identifier",";"});
  syn.addRule({"statement-function"   ,"STATEMENT","TYPE","identifier","(","COMMA-DECL-LIST",")","{","BODY","}"});
  syn.addRule({"statement-return"     ,"STATEMENT","return","EXPRESSION",";"});
  syn.addRule({"statement-expression" ,"STATEMENT","EXPRESSION",";"});
  syn.addRule({"statement-ifelse"     ,"STATEMENT","if","(","EXPRESSION",")","STATEMENT","else","STATEMENT"});
  syn.addRule({"statement-if"         ,"STATEMENT","if","(","EXPRESSION",")","STATEMENT"});
  syn.addRule({"statement-while"      ,"STATEMENT","while","(","EXPRESSION",")","STATEMENT"});
  syn.addRule({"statement-{}"         ,"STATEMENT","{","BODY","}"});

  syn.addRule({"statement-typedef-struct","STATEMENT","typedef","struct","{","DECL-LIST","}","identifier",";"});
  syn.addRule({"statement-typedef-array" ,"STATEMENT","typedef","TYPE","[","integer-value","]","identifier",";"});

  syn.addRule({"decl-list-term","DECL-LIST","TYPE","identifier",";"});
  syn.addRule({"decl-list"     ,"DECL-LIST","TYPE","identifier",";","DECL-LIST"});

  syn.addRule({"comma-decl-list-term","COMMA-DECL-LIST","TYPE","identifier"});
  syn.addRule({"comma-decl-list"     ,"COMMA-DECL-LIST","TYPE","identifier",",","COMMA-DECL-LIST"});

  syn.addRule({"comma-list-term","COMMA-LIST","identifier"});
  syn.addRule({"comma-list"     ,"COMMA-LIST","identifier",",","COMMA-LIST"});

  syn.addRule({"value-float"  ,"VALUE","float-value"  });
  syn.addRule({"value-integer","VALUE","integer-value"});
  syn.addRule({"value-string" ,"VALUE","string-value" });

  syn.addRule({"type-bool"      ,"TYPE","bool"      });
  syn.addRule({"type-i8"        ,"TYPE","i8"        });
  syn.addRule({"type-i16"       ,"TYPE","i16"       });
  syn.addRule({"type-i32"       ,"TYPE","i32"       });
  syn.addRule({"type-i64"       ,"TYPE","i64"       });
  syn.addRule({"type-u8"        ,"TYPE","u8"        });
  syn.addRule({"type-u16"       ,"TYPE","u16"       });
  syn.addRule({"type-u32"       ,"TYPE","u32"       });
  syn.addRule({"type-u64"       ,"TYPE","u64"       });
  syn.addRule({"type-f32"       ,"TYPE","f32"       });
  syn.addRule({"type-f64"       ,"TYPE","f64"       });
  syn.addRule({"type-string"    ,"TYPE","string"    });
  syn.addRule({"type-identifier","TYPE","identifier"});

  syn.addRule({"assign-="  ,"ASSIGN-OPER","="  });
  syn.addRule({"assign-+=" ,"ASSIGN-OPER","+=" });
  syn.addRule({"assign--=" ,"ASSIGN-OPER","-=" });
  syn.addRule({"assign-*=" ,"ASSIGN-OPER","*=" });
  syn.addRule({"assign-/=" ,"ASSIGN-OPER","/=" });
  syn.addRule({"assign-%=" ,"ASSIGN-OPER","%=" });
  syn.addRule({"assign-&=" ,"ASSIGN-OPER","&=" });
  syn.addRule({"assign-|=" ,"ASSIGN-OPER","|=" });
  syn.addRule({"assign-^=" ,"ASSIGN-OPER","^=" });
  syn.addRule({"assign-<<=","ASSIGN-OPER","<<="});
  syn.addRule({"assign->>=","ASSIGN-OPER",">>="});

  syn.addRule({"equality-==","EQUALITY-OPER","=="});
  syn.addRule({"equality-!=","EQUALITY-OPER","!="});

  syn.addRule({"relational-<" ,"RELATIONAL-OPER","<" });
  syn.addRule({"relational->" ,"RELATIONAL-OPER",">" });
  syn.addRule({"relational-<=","RELATIONAL-OPER","<="});
  syn.addRule({"relational->=","RELATIONAL-OPER",">="});

  syn.addRule({"shift-<<","SHIFT-OPER","<<"});
  syn.addRule({"shift->>","SHIFT-OPER",">>"});

  syn.addRule({"additive-+","ADDITIVE-OPER","+"});
  syn.addRule({"additive--","ADDITIVE-OPER","-"});

  syn.addRule({"multiplicative-*","MULTIPLICATIVE-OPER","*"});
  syn.addRule({"multiplicative-/","MULTIPLICATIVE-OPER","/"});
  syn.addRule({"multiplicative-%","MULTIPLICATIVE-OPER","%"});

  syn.addRule({"unary-+","UNARY-OPER","+"});
  syn.addRule({"unary--","UNARY-OPER","-"});
  syn.addRule({"unary-~","UNARY-OPER","~"});
  syn.addRule({"unary-!","UNARY-OPER","!"});

  syn.computeLengths();
  // */
  std::cout<<syn.str()<<std::endl;

  syn.runStart();
  tokenization->begin();
  do{
    if(std::cin.eof())break;
    std::cout<<">>> ";
    std::string line;
    std::getline(std::cin,line);
    if(line == "quit")break;
    if(line == "tokens"){
      for(auto x:syn.ctx.tokens)
        std::cout<<syn._tokenization->tokenName(x.type)<<" ";
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
    line+="\n";
    tokenization->parse(line);
    do{
      if(tokenization->empty())break;
      auto t=tokenization->getToken();
      syn.addToken(t);
    }while(true);
    ge::util::HighResolutionTimer timer;
    timer.reset();
    auto status=syn.runContinue();
    std::cout<<timer.getElapsedTimeSeconds()<<" s"<<std::endl;
    if      (status == ge::core::NodeContext::TRUE){
      //syn.processTree(syn.getSyntaxTree());
      syn.ctx.matches.clear();
      syn.ctx.failedMatches.clear();
      std::cout<<syn.getSyntaxTree()->str()<<std::endl;
      syn.runStart();
    }else if(status == ge::core::NodeContext::FALSE){
      std::cout<<"syntax error"<<std::endl;
      syn.runStart();
    }
  }while(true);
  tokenization->end();
  std::cout<<std::endl;
  return 0;
}
