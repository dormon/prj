#include"textFileLoader.h"
#include<fstream>

BEGIN_INTERPRET_FUNCTION_CPP(TextFileLoader,"string","text","string","fileName");
MID_INTERPRET_FUNCTION_CPP(TextFileLoader);
  if(!this->hasInput(0)||!this->hasOutput())return false;
  if(!this->_inputChanged(0))return false;
  std::ifstream f(((std::string&)*this->getInputData(0)).c_str());
  if(!f.good())return false;
  f.seekg(0,std::ios::end);
  std::size_t size=f.tellg();
  f.seekg(0,std::ios::beg);
  char*buffer=new char[size+1];
  f.read(buffer,size);
  buffer[size]='\0';
  std::string result(buffer);
  delete[]buffer;
  f.close();
  *this->_getOutput().data=result;
  return true;
END_INTERPRET_FUNCTION_CPP();

