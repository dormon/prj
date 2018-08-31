#include <StringAligner/Block.h>

using namespace stringAligner;

void Block::setOnChange(OnChange const&fce){
  onChange = fce;
}

void Block::callOnChange(){
  if(onChange)onChange(this);
}
