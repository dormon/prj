#include<cstdint>
#include<cstdlib>
#include<cassert>
#include<vector>
// Syntax:
//
// Formula         : TermFormula
// Formula         : CompositeFormula
// TermFormula     : identifier
// TermFormula     : ~Formula
// TermFormula     : ( CompositeFormula )
// CompositeFormula: Formula -> Formula
//
// I0,I1,I2,Ii,... are shortcuts for identifiers
// F0,F1,F2,Fi,... are shortcuts for formulas
// _ is any Ii,Fi
//
//
// Axioms:
// F0 -> (F1 -> F0)
// (~F1 -> ~ F0) -> (F0 -> F1)
// (F0 -> (F1 -> F2)) -> ((F0 -> F1) -> (F0 -> F2))
//
// Deriving rule:
// O(F0,F0->F1) = F1
// 
// Substitution rule:
// S(old,where,new)
// S(I0,I0    ,F0) == F0
// S(I0,F0->F1,F2) == S(I0,F0,F2) -> S(I0,F1,F2)
// S(I0,(F0)  ,F1) == (S(I0,F0,F1))
// S(I0,~F0   ,F1) == ~S(I0,F0,F1)
// S(F0,F0    ,F1) == F1
// S(F0,F1->F2,F3) == S(F0,F1,F3) -> S(F0,F2,F3)
// S(F0,(F0)  ,F1) == (F1)
// S(F0,~F0   ,F1) == ~F1
// S(_ ,F0    ,_ ) == F0
//
// Ii == Ii
// Fi == Fi
//
//

uint8_t* allocate(size_t size){
  return new uint8_t[size];
}

void deallocate(uint8_t*ptr){
  delete[]ptr;
}

class Pointer{
  public:
    uint8_t*ptr;
    enum Type{
      DATA   ,
      COMMAND,
    }type;
    Pointer(
        uint8_t*const&p,
        Type    const&t):ptr(p),type(t){}
};

class Command{
  public:
    virtual void operator()() = 0;
    virtual ~Command(){}
};

class BasicCommand: public Command{
  public:
    enum EntryType{
      COMMAND_READS  = 1<<0,
      COMMAND_WRITES = 1<<1,
    };
    std::vector<EntryType>entryTypes;
    BasicCommand(std::vector<EntryType>const&e = {}):entryTypes(e){
    }
};

class AtomicCommand: public BasicCommand{
  public:
    using InstructionType = void(*)(std::vector<Pointer>const&);
    InstructionType     fce ;
    AtomicCommand(
        InstructionType       const&f = nullptr,
        std::vector<EntryType>const&e = {}     ):BasicCommand(e),fce(f){
    }
    /*std::vector<Pointer>data;
    AtomicCommand(
        InstructionType     const&f = nullptr,
        std::vector<Pointer>const&d = {}     ):fce(f),data(d){}
    virtual void operator()()override{
      assert(this      != nullptr);
      assert(this->fce != nullptr);
      this->fce(this->data);
    }*/
    virtual ~AtomicCommand(){}
};

class Entry{
  public:
    Entry(
        size_t const&c = 0,
        size_t const&e = 0):commandIndex(c),entryIndex(e){
    }
    size_t commandIndex;
    size_t entryIndex   ;
};

class EntryInfo{
  public:
    std::vector<Entry>info;
    EntryInfo(std::vector<Entry>const&i):info(i){
    }
    bool check(std::vector<BasicCommand*>cmds)const{
      for(auto const&e:this->info){
        if(e.commandIndex >= cmds.size()){
          return false;
        }
        auto const cmd = cmds.at(e.commandIndex);
        if(e.entryIndex >= cmd->entryTypes.size()){
          return false;
        }
        auto const type = cmd->entryTypes.at(e.entryIndex);
        auto const firstType = cmds.at(info.at(0).commandIndex)->entryTypes.at(info.at(0).entryIndex);
        if(type != firstType){
          return false;
        }
      }
      if(this->info.size() > 1){
        auto const firstType = cmds.at(info.at(0).commandIndex)->entryTypes.at(info.at(0).entryIndex);
        if(firstType != BasicCommand::COMMAND_READS){
          return false;
        }
      }
      return true;
    }
};

class ConnectionInfo{
  public:
    std::vector<EntryInfo>entries;
};

class CommandLayer: public BasicCommand{
  public:
    std::vector<BasicCommand*>commands;
    CommandLayer(
        std::vector<BasicCommand*>const&cmds,
        ConnectionInfo            const&con ):BasicCommand(),commands(cmds){
      assert(this != nullptr);
      assert(cmds.size() == con.entries.size());
      for(auto const&e:con.entries){
        if(e.check(cmds))
          return;
        }

    }

};

class CompositeCommand: public BasicCommand{
  public:
    std::vector<CommandLayer>layers;
};

class CommandList: public Command{
  public:
    std::vector<Command*>commands;
    virtual void operator()()override{
      assert(this != nullptr);
      for(auto const&x:commands){
        assert(x != nullptr);
        (*x)();
      }
    }
    virtual ~CommandList(){
      assert(this != nullptr);
      for(auto const&x:this->commands)
        delete x;
    }
};

void bool_implication(
    std::vector<Pointer>const&a){
  assert(a.size() >= 3);
  assert(a.at(0).ptr != nullptr);
  assert(a.at(1).ptr != nullptr);
  assert(a.at(2).ptr != nullptr);
  assert(a.at(0).type == Pointer::DATA);
  uint8_t*aptr;
  if(a.at(1).type == Pointer::DATA)
    aptr = a.at(0).ptr;
  else{
    (*reinterpret_cast<Command*>(a.at(0).ptr))();
    a = reinterpret_cast<Command*>(a.at(0).ptr)->
  }
  uint8_t*bptr;
  *reinterpret_cast<bool*>(a.at(0).ptr) = 
    ! *reinterpret_cast<bool const*>(a.at(1).ptr) || 
    *reinterpret_cast<bool const*>(a.at(2).ptr)  ;
}

void bool_negation(
    std::vector<Pointer>const&a){
  assert(a.size() >= 3);
  assert(a.at(0) != nullptr);
  assert(a.at(1) != nullptr);
  *reinterpret_cast<bool*>(a.at(0)) = ! *reinterpret_cast<bool const*>(a.at(1));
}



int main(){
  return 0;
}
