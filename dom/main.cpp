#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

namespace Types {

enum class Qualifier {
  IN,
  OUT,
  INOUT,
};

class Type {};

class Empty : public Type {};

class Unqualified : public Type {};

class Qualified : public Type {
  public:
  Qualified(Qualifier const& q) : qualifier(q) {}
  Qualifier getQualifier() const { return qualifier; }

  protected:
  Qualifier qualifier;
};

class Atomic : public Unqualified {};

class Composite : public Unqualified {};

class Any : public Atomic {};

class Bit : public Atomic {};

class Struct : public Composite {
  public:
  using Elements = std::vector<Unqualified>;
  Struct(Elements const& e) : elements(e) {}
  Elements const& getTypes() const { return elements; }

  protected:
  Elements elements;
};

class Vector : public Composite {
  public:
  Vector(Unqualified const& i) : inner(i) {}
  Unqualified getInner() const { return inner; }

  protected:
  Unqualified inner;
};

class Function : public Composite {
  public:
  using Parameters = std::vector<Qualified>;
  Function(Parameters const& p) : parameters(p) {}
  Parameters const& getParameters() const { return parameters; }

  protected:
  Parameters parameters;
};

class Array : public Vector {};

}  // namespace Types



namespace Functions {

class Function {
  public:
  Function(Types::Function const& t) : type(t) {}
  Types::Function getType() const { return type; }

  protected:
  Types::Function type;
};

class Nonpaired : public Function {};

class Atomic : public Nonpaired {
  public:
    enum Kind{
      IS_EMPTY    ,
      IS_PARAMETER,
      IS_IN       ,
      IS_OUT      ,
      IS_ANY      ,
      IS_BIT      ,
      IS_FINITE   ,
      NAND        ,
    };

};

class While : public Nonpaired {};

class If : public While {};

class Paired : public Function {};

class Empty : public Paired {};

class PairFunction : public Paired {
  public:
  protected:
};

}  // namespace Functions

// input memory
// output memory
// local memory
//
// Memory layout:
// |input|output|local|
//
//

// program transformation
// E           -> (A,E)       # change empty function to atomic function
// Ai          -> Aj          # change atomic function to more complex atomic
// function (Ai,(Aj,F)) -> (Aj,(Ai,F)) # change F           -> IF[F]       #
// IF[F]       -> WHILE[F]    #
// A(TABLEi)   -> A(TABLEj)   #
//
//
//
//
//

int main() { return 0; }
