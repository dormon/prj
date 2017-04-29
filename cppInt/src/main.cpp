#include<iostream>
#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
#include<map>

//
// class Statement{};
// class Command    :public Statement{};
// class Call       :public Command{};
// class If         :public Command{};
// class While      :public Command{};
// class Body       :public Command{};
// class Declaration:Statement{};
// class Var        :Declaration{};
// class Model      :Declaration{};
// class 
//
//
//
//
//
//
//


class Statement{
  public:
    enum class Type{
      EXECUTION_STATEMENT ,
      VARIABLE_DECLARATION,
    }type;
    Statement(Type const&t):type(t){}
    virtual void operator()() = 0;
    virtual ~Statement(){}
};

class VariableDeclaration;

class VariableDeclaration: public Statement{
  public:
    enum class Type{
      FLOAT,
      UINT32,
    }type;
    std::string name;
    VariableDeclaration(
        Type        const&t,
        std::string const&n):
      Statement(Statement::Type::VARIABLE_DECLARATION),
      type     (t                                    ),
      name     (n                                    ){}
    virtual void operator()()override final{}
    virtual void*getDataPointer() = 0;
};

class FloatClass: public VariableDeclaration{
  public:
    FloatClass(
        std::string const&name      ,
        float       const&def  = 0.f):
      VariableDeclaration(Type::FLOAT,name),
      _value             (def             ){}
    virtual void*getDataPointer()override{
      assert(this!=nullptr);
      return reinterpret_cast<void*>(&this->_value);
    }
  protected:
    float _value;
};

class Uint32Class: public VariableDeclaration{
  public:
    Uint32Class(
        std::string const&name    ,
        uint32_t    const&def  = 0):
      VariableDeclaration(Type::UINT32,name),
      _value             (def              ){}
    virtual void*getDataPointer()override{
      assert(this!=nullptr);
      return reinterpret_cast<void*>(&this->_value);
    }
  protected:
    uint32_t _value;
};

class ExecutionStatement: public Statement{
  public:
    enum class Type{
      BODY,
      IF  ,
      CALL,
    }type;
    ExecutionStatement(Type const&t):Statement(Statement::Type::EXECUTION_STATEMENT),type(t){}
    virtual void operator()() = 0;
    virtual ~ExecutionStatement(){}
    bool allVariablesBound = false;
    virtual bool bindVariables(std::map<std::string,VariableDeclaration*>const&) = 0;
};



using CommandPointer = void(*)(void*const*const&);

class CallClass: public ExecutionStatement{
  public:
    std::vector<std::string>operandNames;
    std::vector<void*>operandAddresses;
    ///\todo list of types
    template<typename...VARIABLE_NAMES>
      CallClass(CommandPointer const&c,VARIABLE_NAMES const&...vars):ExecutionStatement(Type::CALL),_cmd(c){
        assert(this != nullptr);
        this->operandNames = {vars...};
        this->operandAddresses.resize(this->operandNames.size(),nullptr);
      }
    virtual void operator()()override{
      assert(this != nullptr);
      assert(this->_cmd != nullptr);
      this->_cmd(this->operandAddresses.data());
    }
    virtual ~CallClass(){}
    virtual bool bindVariables(std::map<std::string,VariableDeclaration*>const&vars)override{
      assert(this != nullptr);
      assert(this->operandNames.size() == this->operandAddresses.size());
      this->allVariablesBound = true;
      for(size_t i=0;i<this->operandNames.size();++i){
        if(this->operandAddresses.at(i))continue;
        auto it = vars.find(this->operandNames.at(i));
        if(it == vars.end()){
          this->allVariablesBound = false;
          continue;
        }
        ///\todo typecheck
        this->operandAddresses.at(i) = it->second->getDataPointer();
      }
      return this->allVariablesBound;
    }
  protected:
    CommandPointer _cmd;
};


class BodyClass: public ExecutionStatement{
  public:
    virtual void operator()()override{
      assert(this!=nullptr);
      for(auto const&x:this->_statements)
        (*x)();
    }
    template<typename...STATEMENTS>
      BodyClass(STATEMENTS const&...s):ExecutionStatement(Type::BODY),_parentBody(nullptr){
        assert(this!=nullptr);
        this->_statements = {s...};
        for(auto const&x:this->_statements){
          if(x->type == Statement::Type::VARIABLE_DECLARATION){
            auto const v = reinterpret_cast<VariableDeclaration*>(x);
            auto it = this->_variables.find(v->name);
            if(it != this->_variables.end()){
              throw("Variable: "+v->name+" is already declared!");
            }
            this->_variables[v->name] = v;
          }else{
            auto const e = reinterpret_cast<ExecutionStatement*>(x);
            (void)e;
          }
        }
      }
    ~BodyClass(){
      assert(this!=nullptr);
      for(auto const&x:this->_statements)
        delete x;
    }
    virtual bool bindVariables(std::map<std::string,VariableDeclaration*>const&vars)override{
      assert(this != nullptr);
      this->allVariablesBound = true;
      for(auto const&x:this->_statements){
        if(x->type == Statement::Type::VARIABLE_DECLARATION)continue;
        auto const e = reinterpret_cast<ExecutionStatement*>(x);
        if(e->allVariablesBound)continue;
        this->allVariablesBound &= e->bindVariables(vars);
      }
      return this->allVariablesBound;
    }
  protected:
    std::vector<Statement*>_statements;
    std::map<std::string,VariableDeclaration*>_variables;
    BodyClass*_parentBody;
};

class IfClass: public ExecutionStatement{
  public:
    IfClass(
        std::string        const&condition          ,
        ExecutionStatement*const&tPath              ,
        ExecutionStatement*const&fPath     = nullptr):
      ExecutionStatement(ExecutionStatement::Type::IF),
      conditionVariableName(condition),
      truePath(tPath),
      falsePath(fPath){}
    std::string conditionVariableName;
    Statement*truePath ;
    Statement*falsePath;
    void*variablePointer = nullptr;
    void operator()()override{
      assert(this != nullptr);
      assert(this->variablePointer != nullptr);
      assert(this->truePath != nullptr);
      if(*reinterpret_cast<uint32_t*>(this->variablePointer))
        (*this->truePath)();
      else{
        if(this->falsePath)(*this->falsePath)();
      }
    }
    virtual bool bindVariables(std::map<std::string,VariableDeclaration*>const&vars)override{
      assert(this != nullptr);
      this->allVariablesBound = true;
      if(!this->variablePointer){
        auto it = vars.find(this->conditionVariableName);
        if(it == vars.end())
          this->allVariablesBound &= false;
      }

      return this->allVariablesBound;
    }
};

Statement*Float(std::string const&name,float const&def = 0.f){
  return new FloatClass(name,def);
}

Statement*Uint32(std::string const&name,uint32_t const&def = 0){
  return new Uint32Class(name,def);
}

template<typename...STATEMENTS>
BodyClass*Body(STATEMENTS const&...s){
  return new BodyClass(s...);
}

int main(){

  return 0;
}
