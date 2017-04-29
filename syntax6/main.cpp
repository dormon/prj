#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<geCore/syntax.h>
#include<geCore/syntaxTreeProcessor.h>
#include<geUtil/HighResolutionTimer.h>

#include<geCore/lexer.h>
#include<geUtil/NamespaceWithUsers.h>
#include<geCore/text.h>
#include<geCore/TypeRegister.h>
#include<geCore/Accessor.h>
#include<geCore/functionRegister.h>
#include<geCore/stdFunctions.h>
#include<geCore/macroFunction.h>
#include<limits>

class Kernel{
  public:
    std::shared_ptr<ge::core::TypeRegister>typeRegister = nullptr;
    std::shared_ptr<ge::core::Namer>namer = nullptr;
    std::shared_ptr<ge::core::FunctionRegister>functionRegister = nullptr;
    std::shared_ptr<ge::core::sim::Namespace>vars = nullptr;
    Kernel(){
      this->typeRegister = std::make_shared<ge::core::TypeRegister>();
      this->namer = std::make_shared<ge::core::Namer>();
      this->functionRegister = std::make_shared<ge::core::FunctionRegister>(this->typeRegister,this->namer);
      ge::core::registerStdFunctions(this->functionRegister);
      this->vars = std::make_shared<ge::core::sim::Namespace>("*");
    }

    ge::core::TypeRegister::TypeID type = 0;

    std::vector<ge::core::TypeRegister::TypeID>typeList;
    std::vector<std::string>nameList;

    std::shared_ptr<ge::core::StatementFactory>s0 = nullptr;
    std::shared_ptr<ge::core::StatementFactory>s1 = nullptr;

    static std::string chopQuotes(std::string str){
      return str.substr(0,str.length()-1).substr(1);
    }

    static inline std::shared_ptr<ge::core::NontermNode>toN(ge::core::SyntaxNode::Node node){
      return std::dynamic_pointer_cast<ge::core::NontermNode>(node);
    }

    static inline std::shared_ptr<ge::core::TermNode>toT(ge::core::SyntaxNode::Node node){
      return std::dynamic_pointer_cast<ge::core::TermNode>(node);
    }

    static void kernelVars(ge::core::SyntaxNode::Node,void*data){
      assert(data!=nullptr);
      auto k=(Kernel*)data;
      assert(k->vars!=nullptr);
      std::cout<<k->vars->toStr(0);
    }



    static void getType(ge::core::SyntaxNode::Node node,void*data){
      assert(node!=nullptr);
      assert(data!=nullptr);
      auto k=(Kernel*)data;
      assert(0<toN(node)->childs.size());
      assert(toN(node)->childs[0]!=nullptr);
      auto typeName = toT(toN(node)->childs[0])->token.rawData;
      assert(k->typeRegister!=nullptr);
      k->type = k->typeRegister->getTypeId(typeName);
    }

    static void addVar(ge::core::SyntaxNode::Node node,void*d){
      assert(node!=nullptr);
      assert(d!=nullptr);
      assert(1<toN(node)->childs.size());
      assert(toN(node)->childs[1]!=nullptr);
      auto k=(Kernel*)d;
      if(k->type == ge::core::TypeRegister::getTypeTypeId<ge::core::TypeRegister::Unregistered>()){
        std::cerr<<"ERROR: addVariable() - wrong type"<<std::endl;
        return;
      }

      assert(k->vars!=nullptr);
      assert(k->typeRegister!=nullptr);

      auto varName = toT(toN(node)->childs[1])->token.rawData;
      std::cout<<varName<<std::endl;
      k->vars->insert(varName,k->typeRegister->sharedAccessor(k->type));
    }

    static void registerArrayType(ge::core::SyntaxNode::Node node,void*d){
      assert(node!=nullptr);
      assert(d!=nullptr);
      assert(3<toN(node)->childs.size());
      assert(toN(node)->childs[3]!=nullptr);
      assert(5<toN(node)->childs.size());
      assert(toN(node)->childs[5]!=nullptr);
      auto k=(Kernel*)d;
      if(k->type == ge::core::TypeRegister::getTypeTypeId<ge::core::TypeRegister::Unregistered>()){
        std::cerr<<"ERROR: registerArrayType() - wrong inner type"<<std::endl;
        return;
      }
      auto size = std::atoi(toT(toN(node)->childs[3])->token.rawData.c_str());
      auto name = toT(toN(node)->childs[5])->token.rawData;
      ge::core::TypeRegister::DescriptionList registrationData;
      registrationData.push_back(ge::core::TypeRegister::ARRAY);
      registrationData.push_back(size);
      registrationData.push_back(k->type);
      k->typeRegister->addType(name,registrationData);
    }

    static void registerStructTypePre(ge::core::SyntaxNode::Node node,void*d){
      assert(node!=nullptr);
      assert(d!=nullptr);
      auto k=(Kernel*)d;
      k->typeList.clear();
      k->nameList.clear();
    }

    static void addDeclList(ge::core::SyntaxNode::Node node,void*d){
      assert(node!=nullptr);
      assert(d!=nullptr);
      auto k=(Kernel*)d;
      k->typeList.push_back(k->type);
      k->nameList.push_back(toT(toN(node)->childs[1])->token.rawData);
    }

    static void registerStructType(ge::core::SyntaxNode::Node node,void*d){
      assert(node!=nullptr);
      assert(d!=nullptr);
      assert(5<toN(node)->childs.size());
      assert(toN(node)->childs[5]!=nullptr);
      auto k=(Kernel*)d;
      auto name = toT(toN(node)->childs[5])->token.rawData;
      ge::core::TypeRegister::DescriptionList rdata;
      rdata.push_back(ge::core::TypeRegister::STRUCT);
      rdata.push_back(k->typeList.size());
      for(auto x:k->typeList)
        rdata.push_back(x);
      k->typeRegister->addType(name,rdata);
    }

    static void expressionOr(ge::core::SyntaxNode::Node node,void*d){
      assert(node!=nullptr);
      assert(d!=nullptr);
      auto k=(Kernel*)d;
      /*
      auto fac = std::make_shared<ge::core::FunctionNodeFactory>("",1);
      fac->setFactory(k->functionRegister->sharedFactory("Or"));
      fac->addResourceFactory(std::make_shared<ge::core::ResourceFactory>(k->typeRegister->getTypeId()));
      fac->addInputFactory(k->s0);
      fac->addInputFactory(k->s1);
      */
    }

};

int main(){
  //TODO lvalue for assignment operators
  //TODO optional initialization of variables
  //TODO access members of arrays and structures

  Kernel kernel;

  ge::core::Syntax syn(ge::core::loadTextFile("/home/dormon/Desktop/lex2.txt"),ge::core::loadTextFile("/home/dormon/Desktop/syn2.txt"));
  std::cout<<syn.str()<<std::endl;

  ge::core::SyntaxTreeProcessor processor;
  processor.addCallback("STATEMENT","statement-kernel-vars",nullptr,Kernel::kernelVars,&kernel);
  processor.addCallback("TYPE","",nullptr,Kernel::getType,&kernel);
  processor.addCallback("STATEMENT","statement-declaration",nullptr,Kernel::addVar,&kernel);
  processor.addCallback("STATEMENT","statement-typedef-array",nullptr,Kernel::registerArrayType,&kernel);
  processor.addCallback("STATEMENT","statement-typedef-struct",Kernel::registerStructTypePre,Kernel::registerStructType,&kernel);
  processor.addCallback("DECL-LIST","decl-list-term",nullptr,Kernel::addDeclList,&kernel);
  processor.addCallback("DECL-LIST","decl-list",nullptr,Kernel::addDeclList,&kernel);
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
