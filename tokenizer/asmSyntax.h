#pragma once

#include<geCore/token.h>

namespace ge{
  namespace core{
    class AsmSyntax{
      public:
        AsmSyntax();
        ~AsmSyntax();
        void STATEMENT(Token const&t);
        void addToken(Token const&t);
    };
  }
}
