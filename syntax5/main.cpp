#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<geCore/registerStdFunctions.h>
#include<geCore/producer.h>
#include<geCore/syntax.h>
#include<geCore/syntaxTreeProcessor.h>
#include<geUtil/HighResolutionTimer.h>

#include<geCore/lexer.h>
#include<geUtil/NamespaceWithUsers.h>
#include<geCore/text.h>
#include<limits>

int main(){
  //TODO lvalue for assignment operators
  //TODO optional initialization of variables
  //TODO access members of arrays and structures

  ge::core::Syntax syn(ge::core::loadTextFile("/home/dormon/Desktop/lex2.txt"),ge::core::loadTextFile("/home/dormon/Desktop/syn2.txt"));
  std::cout<<syn.str()<<std::endl;

  ge::core::SyntaxTreeProcessor processor;
  processor.addCallback("TYPE","",nullptr,[](ge::core::SyntaxNode::Node n,void*){
      std::cout<<std::dynamic_pointer_cast<ge::core::NontermNode>(n)->childs[0]->getTerm()->str()<<" typ: f32"<<std::endl;},nullptr);
  syn.begin();
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
    ge::util::HighResolutionTimer timer;
    timer.reset();
    auto result = syn.parse(line);
    std::cout<<timer.getElapsedTimeSeconds()<<" s"<<std::endl;
    if(result.first == ge::core::NodeContext::TRUE_STATUS){
      std::cout<<result.second->str()<<std::endl;
      processor(result.second);
    }
  }while(true);
  syn.end();
  std::cout<<std::endl;
  return 0;
}
