#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include<geCore/fsa/fsa.h>

class ParseEnumArgs{
  protected:
    struct ParserData{
      unsigned      start;
      std::string   key  ;
      ParseEnumArgs*_this;
      unsigned      id   ;
    }_pData;
    static void _begin         (ge::core::FSA*fsa,void*data);
    static void _writeKey      (ge::core::FSA*fsa,void*data);
    static void _storeKey      (ge::core::FSA*fsa,void*data);
    static void _writeStoredKey(ge::core::FSA*fsa,void*data);
    static void _writeValue    (ge::core::FSA*fsa,void*data);
    std::shared_ptr<ge::core::FSA>_f = nullptr;
    std::map<unsigned,std::string>_id2Name;
  public:
    ParseEnumArgs();
    std::map<unsigned,std::string>operator()(const char*args);
};

void ParseEnumArgs::_begin(ge::core::FSA*fsa,void*data){
  ParserData*p=(ParserData*)data;
  p->start = fsa->getCurrentPosition();
}

void ParseEnumArgs::_writeKey(ge::core::FSA*fsa,void*data){
  ParserData*p=(ParserData*)data;
  p->_this->_id2Name[p->id++]=fsa->getAlreadyReadString().substr(p->start);
}

void ParseEnumArgs::_storeKey(ge::core::FSA*fsa,void*data){
  ParserData*p=(ParserData*)data;
  p->key = fsa->getAlreadyReadString().substr(p->start);
}

void ParseEnumArgs::_writeStoredKey(ge::core::FSA*fsa,void*data){
  ParserData*p=(ParserData*)data;
  p->_this->_id2Name[p->id++]=p->key;
}

void ParseEnumArgs::_writeValue(ge::core::FSA*fsa,void*data){
  ParserData*p=(ParserData*)data;
  std::string sValue=fsa->getAlreadyReadString().substr(p->start);
  p->id = std::atoi(sValue.c_str());
  p->_this->_id2Name[p->id++] = p->key;
}


ParseEnumArgs::ParseEnumArgs(){
  this->_f = std::make_shared<ge::core::FSA>(
      "S",
      "S",ge::core::FSA::space,"S",
      "S","a\\-zA\\-Z_"       ,"A",(ge::core::FSACallback::Fce)this->_begin         ,(void*)&this->_pData,
      "S",ge::core::FSA::eof  ,"H",(ge::core::FSACallback::Fce)this->_writeKey      ,(void*)&this->_pData,
      "A","a\\-zA\\-Z0\\-9_"  ,"A",
      "A",","                 ,"B",(ge::core::FSACallback::Fce)this->_writeKey      ,(void*)&this->_pData,
      "A",ge::core::FSA::space,"C",(ge::core::FSACallback::Fce)this->_storeKey      ,(void*)&this->_pData,
      "A","="                 ,"D",(ge::core::FSACallback::Fce)this->_storeKey      ,(void*)&this->_pData,
      "A",ge::core::FSA::eof  ,"H",(ge::core::FSACallback::Fce)this->_writeKey      ,(void*)&this->_pData,
      "B",ge::core::FSA::space,"S",
      "B","a\\-zA\\-Z_"       ,"A",(ge::core::FSACallback::Fce)this->_begin         ,(void*)&this->_pData,
      "B",ge::core::FSA::eof  ,"H",
      "C",ge::core::FSA::space,"C",
      "C",","                 ,"B",(ge::core::FSACallback::Fce)this->_writeStoredKey,(void*)&this->_pData,
      "C","="                 ,"D",
      "C",ge::core::FSA::eof  ,"H",(ge::core::FSACallback::Fce)this->_writeStoredKey,(void*)&this->_pData,
      "D",ge::core::FSA::space,"E",
      "D",","                 ,"B",(ge::core::FSACallback::Fce)this->_writeStoredKey,(void*)&this->_pData,
      "D",ge::core::FSA::digit,"F",(ge::core::FSACallback::Fce)this->_begin         ,(void*)&this->_pData,
      "D",ge::core::FSA::eof  ,"H",(ge::core::FSACallback::Fce)this->_writeStoredKey,(void*)&this->_pData,
      "E",ge::core::FSA::space,"E",
      "E",","                 ,"B",(ge::core::FSACallback::Fce)this->_writeStoredKey,(void*)&this->_pData,
      "E",ge::core::FSA::digit,"F",(ge::core::FSACallback::Fce)this->_begin         ,(void*)&this->_pData,
      "E",ge::core::FSA::eof  ,"H",(ge::core::FSACallback::Fce)this->_writeStoredKey,(void*)&this->_pData,
      "F",ge::core::FSA::digit,"F",
      "F",ge::core::FSA::space,"G",(ge::core::FSACallback::Fce)this->_writeValue    ,(void*)&this->_pData,
      "F",","                 ,"B",(ge::core::FSACallback::Fce)this->_writeValue    ,(void*)&this->_pData,
      "F",ge::core::FSA::eof  ,"H",(ge::core::FSACallback::Fce)this->_writeValue    ,(void*)&this->_pData,
      "G",ge::core::FSA::space,"G",
      "G",","                 ,"B",
      "G",ge::core::FSA::eof  ,"H"
        );
  this->_pData._this = this;
}

std::map<unsigned,std::string>ParseEnumArgs::operator()(const char*args){
  this->_pData.key="";
  this->_pData._this = this;
  this->_pData.start = 0;
  this->_pData.id = 0;
  this->_f->run(std::string(args));
  return this->_id2Name;
}

#define VA_ARGS_TO_STRING(...)\
  #__VA_ARGS__

#undef DEF_ENUM

#define DEF_ENUM(name,...)\
  enum name{\
    __VA_ARGS__\
  };\
friend std::ostream& operator<<(std::ostream& os,const name&val){\
  static std::map<unsigned,std::string>table = ParseEnumArgs()(VA_ARGS_TO_STRING(__VA_ARGS__));\
  auto a=table.find(val);\
  if(a!=table.end())os << a->second;\
  return os;\
}

class Moj{
  public:
    DEF_ENUM(Type,AA=32,BB,CC,DD,EE,FF);
};

int main(){
  std::cout<<Moj::AA<<std::endl;
  std::cout<<Moj::BB<<std::endl;
  return 0;
}





