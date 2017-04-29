#include"fileExtensionExtractor.h"

BEGIN_INTERPRET_FUNCTION_CPP(FileExtensionExtractor,"string","extension","string","fileName");
MID_INTERPRET_FUNCTION_CPP(FileExtensionExtractor);
  if(!this->hasInput(0)||!this->hasOutput())return false;
  if(!this->_inputChanged(0))return false;
  std::string fileName=*this->getInputData(0);
  auto pos = fileName.rfind(".");
  std::string ext="";
  if(pos!=std::string::npos)
    ext=fileName.substr(pos+1);
  (std::string&)(*this->_getOutput().data)=ext;
  return true;
END_INTERPRET_FUNCTION_CPP();

