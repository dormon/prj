#include <array>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>
#include <tuple>

class Type                                                              {                                                                         };
class Unqualified      :public  Type                                    {                                                                         };
class Atomic           :public  Unqualified                             {                                                                         };
class Any              :public  Atomic                                  {                                                                         };
class Bit              :public  Atomic                                  {                                                                         };
class Composite        :public  Unqualified                             {                                                                         };
class Vector           :public  Composite                               {public:std::unique_ptr<Unqualified>inner;                                };
class Struct           :public  Composite                               {public:std::unique_ptr<Unqualified>head;std::unique_ptr<Struct>tail;     };
class Argument         :public  Type                                    {                                                                         };
class Qualifier                                                         {public:enum{IN,OUT,INOUT};                                               };
class AtomicArgument   :public  Argument                                {public:std::unique_ptr<Unqualified>inner;Qualifier qualifier;            };
class CompositeArgument:public  Argument                                {                                                                         };
class VectorArgument   :public  CompositeArgument                       {public:std::unique_ptr<Argument>inner;                                   };
class StructArgument   :public  CompositeArgument                       {public:std::unique_ptr<Argument>head;std::unique_ptr<StructArgument>tail;};
class Empty            :private Struct           ,private StructArgument{                                                                         };
//isArgument
//isAtomic
//
//

// nand(o,a,b) <=> nand(o,b,a)
//
// {                  {
//   nand(o,a,b)        nand(o,a,c)
//   nand(p,o,c) <!=>   nand(p,o,b)
// }                  }
//
// o=!(a&b)
// p=!(o&c)
// p=!((!(a&b))&c)
// p=(!(!(a&b)))|(!c)
// p=(a&b)|(!c)
//
// o=!(a&c)
// p=!(o&b)
// p=!((!(a&c))&b)
// p=(!(!(a&c))|(!b)
// p=(a&c)|(!b)
//
//
//
//
//
//
//
//

int main() {
  return 0; 
}
