#include"fileExtensionTranslator.h"

BEGIN_INTERPRET_FUNCTION_CPP(FileExtensionTranslator,"u32","type","string","fileExtension","string","config");
  this->_parser = std::make_shared<ge::core::FSA>(
      "START"   ,
      "START"   ,ge::core::FSA::space,"START"   ,
      "START"   ,"#"                 ,"COMMENT0",
      "START"   ,ge::core::FSA::els  ,"KEY"     ,(ge::core::FSACallback::Fce)this->_begin,(void*)&this->_pData,
      "START"   ,ge::core::FSA::eof  ,"END"     ,

      "COMMENT0","\r\n"              ,"START"   ,
      "COMMENT0",ge::core::FSA::els  ,"COMMENT0",
      "COMMENT0",ge::core::FSA::eof  ,"END"     ,

      "KEY"     ,ge::core::FSA::space,"SPACE"   ,(ge::core::FSACallback::Fce)this->_grabKey,(void*)&this->_pData,
      "KEY"     ,ge::core::FSA::els  ,"KEY"     ,
      "KEY"     ,ge::core::FSA::eof  ,"END"     ,

      "SPACE"   ,ge::core::FSA::space,"SPACE"   ,
      "SPACE"   ,"#"                 ,"COMMENT1",
      "SPACE"   ,ge::core::FSA::els  ,"VALUE"   ,(ge::core::FSACallback::Fce)this->_begin,(void*)&this->_pData,
      "SPACE"   ,ge::core::FSA::eof  ,"END"     ,

      "COMMENT1","\r\n"              ,"SPACE1"  ,
      "COMMENT1",ge::core::FSA::els  ,"COMMENT1",
      "COMMENT1",ge::core::FSA::eof  ,"END"     ,

      "VALUE"   ,ge::core::FSA::space,"START"   ,(ge::core::FSACallback::Fce)this->_grabValue,(void*)&this->_pData,
      "VALUE"   ,ge::core::FSA::els  ,"VALUE"   ,
      "VALUE"   ,ge::core::FSA::eof  ,"END"     ,(ge::core::FSACallback::Fce)this->_grabValue,(void*)&this->_pData
      );
MID_INTERPRET_FUNCTION_CPP(FileExtensionTranslator);
  if(!this->hasInput(0)||!this->hasInput(1)||!this->hasOutput())return false;
  std::string file=*this->getInputData(0);
  bool changed=false;
  if(this->_inputChanged("config")){
    this->_table.clear();
    std::string txt=*this->getInputData(1);
    this->_pData._this = this;
    this->_parser->run(txt);
    changed=true;
  }
  if(changed||this->_inputChanged("fileExtension")){
    auto ii=this->_table.find((std::string&)*this->getInputData("fileExtension"));
    unsigned num=-1;
    if(ii!=this->_table.end())
      num=ii->second;
    (unsigned&)(*this->_getOutput().data)=num;
    return true;
  }
  return false;
END_INTERPRET_FUNCTION_CPP();



void FileExtensionTranslator::_begin     (ge::core::FSA*fsa,void*data){
  ParserData*p=(ParserData*)data;
  p->start = fsa->getCurrentPosition();
}

void FileExtensionTranslator::_grabKey   (ge::core::FSA*fsa,void*data){
  ParserData*p=(ParserData*)data;
  p->key = fsa->getAlreadyReadString().substr(p->start);
}

void FileExtensionTranslator::_grabValue (ge::core::FSA*fsa,void*data){
  ParserData*p=(ParserData*)data;
  std::string sValue=fsa->getAlreadyReadString().substr(p->start);
  for(unsigned i=0;i<sValue.size();++i)
    if(sValue[i]<'0'||sValue[i]>'9'){
      std::cerr<<"ERROR: "<<"FileExtensionTranslator::_grabValue() - key ";
      std::cerr<<"\""<<p->key<<"\" has wrong value: \""<<sValue<<"\""<<std::endl;
      return;
    }
  p->_this->_table[p->key]=std::atoi(sValue.c_str());
}


