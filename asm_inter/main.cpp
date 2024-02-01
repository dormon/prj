#include<iostream>
#include<sstream>
#include<vector>
#include<memory>

class Instruction{
  public:
    enum Type{
      ASS,  // =
      ADD,  // +
      SUB,  // -
      MUL,  // *
      LESS, // <
      JMP,  // jump
      JNZ,  // jump if not zero
      PUSH, // push function_id parameter_id value
      CALL, // call function_id
      GET_RESULT_FROM_SUBFUNCTION , // function_id, address
      MOV , // move data
    }type;
    uint32_t params[3];

    Instruction(Type t,uint32_t a = 0,uint32_t b = 0,uint32_t c = 0){
      type = t;
      params[0] = a;
      params[1] = b;
      params[2] = c;
    }
};

// data[0] - return value
// data[1] - 0. parameter
// data[2] - 1. parameter
// data[3] - 2. parameter
// ....
// data[i] - local variable
class Function{
  public:
  
    std::vector<Function>subFunctions;

    std::vector<uint32_t>data;
    std::vector<Instruction>insts;

    void execute(){
      uint32_t pc = 0;
      while(true){
        if(pc >= insts.size())return;

        auto inst = insts.at(pc);
        pc++;//next instruction;
 
        if(inst.type == Instruction::ASS){
          auto r = inst.params[0];
          auto a = inst.params[1];
          data.at(r) = a;
        }

        if(inst.type == Instruction::MOV){
          auto a = inst.params[0];
          auto b = inst.params[1];
          data.at(a) = data.at(b);
        }

        if(inst.type == Instruction::ADD){
          auto r = inst.params[0];
          auto a = inst.params[1];
          auto b = inst.params[2];
          data.at(r) = data.at(a) + data.at(b);
        }
        if(inst.type == Instruction::SUB){
          auto r = inst.params[0];
          auto a = inst.params[1];
          auto b = inst.params[2];
          data.at(r) = data.at(a) - data.at(b);
        }
        if(inst.type == Instruction::MUL){
          auto r = inst.params[0];
          auto a = inst.params[1];
          auto b = inst.params[2];
          data.at(r) = data.at(a) * data.at(b);
        }

        if(inst.type == Instruction::LESS){
          auto r = inst.params[0];
          auto a = inst.params[1];
          auto b = inst.params[2];
          data.at(r) = (uint32_t)(data.at(a) < data.at(b));
        }

        if(inst.type == Instruction::JMP){
          pc = inst.params[0];
        }

        if(inst.type == Instruction::JNZ){
          auto condition = inst.params[1];
          if(data.at(condition) != 0)
            pc = inst.params[0];
        }

        if(inst.type == Instruction::CALL){
          auto function_id = inst.params[0];
          subFunctions.at(function_id).execute();
        }

        if(inst.type == Instruction::PUSH){
          auto function_id = inst.params[0];
          auto param_id    = inst.params[1];
          auto src_id      = inst.params[2];
          subFunctions.at(function_id).data[param_id+1] = data[src_id];
        }

        if(inst.type == Instruction::GET_RESULT_FROM_SUBFUNCTION){
          auto function_id = inst.params[0];
          auto a           = inst.params[1];
          auto result = subFunctions.at(function_id).data.at(0);
          data.at(a) = result;
        };
      }
    }
};

int main(){
  //int a=3;
  //int b=a*a;
  //int c=b*a;
 
  {
  Function fce;
  fce.data.resize(1000);
  fce.insts.push_back(Instruction(Instruction::ASS,0,3  )); // a = 3     --- a %0
  fce.insts.push_back(Instruction(Instruction::MUL,1,0,0)); // b = a * a --- b %1
  fce.insts.push_back(Instruction(Instruction::MUL,2,1,0)); // c = b * a --- c %2

  fce.execute();
  }



  
  {
  //int my_func(int a,int b,int c){
  //  int d = a-b;
  //  int e = c-b;
  //  int f = d*e;
  //  return f;
  //}
  //
  //int main(){
  //  int a=13;
  //  int b=12;
  //  int c=17;
  //  int d = my_func(a,b,c);
  //}
  //
    Function fce;
    fce.data.resize(1000);
    fce.insts.push_back(Instruction(Instruction::ASS,0,13)); // a = 13 --- a %0
    fce.insts.push_back(Instruction(Instruction::ASS,1,12)); // b = 12 --- b %1
    fce.insts.push_back(Instruction(Instruction::ASS,2,17)); // c = 17 --- a %2

    fce.insts.push_back(Instruction(Instruction::PUSH,0,0,0)); // push a to 0. parameter of 0 subfunction
    fce.insts.push_back(Instruction(Instruction::PUSH,0,1,1)); // push b to 1. parameter of 0 subfunction
    fce.insts.push_back(Instruction(Instruction::PUSH,0,2,2)); // push c to 2. parameter of 0 subfunction
    fce.insts.push_back(Instruction(Instruction::CALL,0)); // call my_func(a,b,c)
    fce.insts.push_back(Instruction(Instruction::GET_RESULT_FROM_SUBFUNCTION,0,3)); // int d = my_func.result;
  


    fce.subFunctions.push_back({}); // create sub function
    auto&my_func = fce.subFunctions.back();
    my_func.data.resize(1000);
    my_func.insts.push_back(Instruction(Instruction::SUB,4,1,2)); // d = a-b
    my_func.insts.push_back(Instruction(Instruction::SUB,5,3,2)); // e = c-b
    my_func.insts.push_back(Instruction(Instruction::MUL,6,4,5)); // f = d*e
    my_func.insts.push_back(Instruction(Instruction::MOV,0,6)); // return f --- move f to 0. data element

    
    fce.execute();

    for(uint32_t i=0;i<10;++i)
      std::cerr << fce.data[i] << std::endl;


    std::cerr << "sub fuction memory:" << std::endl;
    for(uint32_t i=0;i<10;++i)
      std::cerr << fce.subFunctions.at(0).data[i] << std::endl;



  }


  return 0;
}
