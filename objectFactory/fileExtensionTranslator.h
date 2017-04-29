#pragma once

#include<geCore/interpret.h>
#include<geCore/stdFunctions.h>
#include<geCore/fsa/fsa.h>

BEGIN_INTERPRET_FUNCTION_HPP(FileExtensionTranslator);
  protected:
    std::map<std::string,unsigned>_table;
    std::shared_ptr<ge::core::FSA>_parser = nullptr;
    struct ParserData{
      unsigned start;
      std::string key;
      FileExtensionTranslator*_this;
    }_pData;
    static void _begin    (ge::core::FSA*fsa,void*data);
    static void _grabKey  (ge::core::FSA*fsa,void*data);
    static void _grabValue(ge::core::FSA*fsa,void*data);
END_INTERPRET_FUNCTION_HPP();

