#pragma once

#include<iostream>

namespace ge{
  namespace util{
    class Lex2{
      private:
        void*    _data            ;
        unsigned _size            ;
        bool     _shouldDeallocate;
      public:
        Lex2(void*data=NULL,unsigned size=0,bool shouldDeallocate=false);
        ~Lex2();
        void*    getData();
        unsigned getSize();
        int      cmp(const void*data,unsigned size,unsigned offset);
        int      cmp           (Lex2&b);
        int      cmpPrefix     (Lex2&b);
        bool     isSpaceBetween(Lex2&b);
        std::string toStr();
    };
  }
}
