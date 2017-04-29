#include"langobjects.h"

using namespace lang;

Ladder::Ladder(
    float barSize,
    float standSize,
    unsigned division,
    std::string systemName,
    std::string outputSystemName,
    std::string ruleName):lang::Rule(ruleName,systemName){
  this->barSize=barSize;
  this->standSize=standSize;
  this->division=division;
  this->outputSystemName=outputSystemName;
}

lang::String*Ladder::body(lang::Generator*gen){
  lang::String*result=new lang::String();
  //SystemSymbol*parent=(SystemSymbol*)gen->currentString->string[gen->currentPosition];


/*
  result->string.push_back(
      new SystemSymbol(
        parent->corner,
        parent->x,
        parent->y,
        parent->z,
        this->outputSystemName));

  for(unsigned x=0;x<this->division.x;++x){
      result->string.push_back(
          new SystemSymbol(
            parent->corner+
            parent->x*(float)x/(float)this->division.x+
            parent->y*(float)y/(float)this->division.y,
            parent->x/(float)this->division.x,
            parent->y/(float)this->division.y,
            parent->z,
            this->outputSystemName));
  }*/
  return result;
}
/*
Stairs::Stairs(
    unsigned num,
    float overlap,
    std::string systemName,
    std::string outputSystemName,
    std::string ruleName){
  this->num=num;
  this->overlap;
  this->outputSystemName;
}
lang::String*Stairs::body(lang::Generator*gen){
  lang::String*result=new lang::String();
  SystemSymbol*parent=(SystemSymbol*)gen->currentString->string[gen->currentPosition];
  for(unsigned k=0;k<this->num;++k){
    result->string.push_back(
        parent->corner+parent->y*((float)k/this->num)+parent->z*((float)k/this->num),
        parent->x,
        parent->y/(float)this->num,
        parent->z/(float)this->num*(float)(1+this->overlap*(k<this->num-1)),
        this->outputSystemName
        );
  }
  return result;

}
*/

