#include"callExtensionTranslator.h"
#include"fileExtensionTranslator.h"

BEGIN_INTERPRET_FUNCTION_CPP(CallExtensionTranslator,
    "u32","id",
    "string","fileExtension",
    "string2u32","fileExtensionTranslator");
MID_INTERPRET_FUNCTION_CPP(CallExtensionTranslator);
  if(!this->hasInput(0)||!this->hasInput(1))return false;
  if(!this->_inputChanged(0)&&!this->_inputChanged(1))return false;
  auto atr=this->getInputData("fileExtensionTranslator");
  auto tr=((std::shared_ptr<ge::core::Function>&)*atr);
  tr->bindInput("fileExtension",this->_inputs[0].function);
  tr->bindOutput(this->_getOutput().data);
  (*tr)();
  return true;
END_INTERPRET_FUNCTION_CPP();


