#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<functional>
#include<stdexcept>
#include<cassert>
#include<sstream>

namespace base{
  class Any;
};

class base::Any{};

namespace relations{
  class Relation;
};

class relations::Relation{
  public:
    size_t dimension;
    Relation(size_t d):dimension(d){}
    Relation():dimension(0){}
};

namespace bools{
  class Bool ;
  enum class BoolKind{
    FALSE_BOOL,
    TRUE_BOOL,
  };
  class False;
  class True ;
  class Relations;
  class NandRelation;
  bool trueRelations(
};

class bools::Bool{
  public:
    BoolKind kind;
    Bool(BoolKind const&k):kind(k){}
};

class bools::False: public Bool{
  public:
    False():Bool(BoolKind::FALSE_BOOL){}
};

class bools::True: public Bool{
  public:
    True():Bool(BoolKind::TRUE_BOOL){}
};

namespace sets{
  class Set;
  class EmptySet;
  class CommonSet;
  enum class SetKind{
    EMPTY  ,
    COMMON ,
  };
  void insert(std::shared_ptr<Set>const&c,std::shared_ptr<Set>const&e);
  std::shared_ptr<Set>unionSet(std::shared_ptr<Set>const&a,std::shared_ptr<Set>const&b);
}

class sets::Set{
  public:
    SetKind kind;
    std::set<std::shared_ptr<Set>>elements;
    Set(SetKind const&k):kind(k){}
};

void sets::insert(std::shared_ptr<Set>const&c,std::shared_ptr<Set>const&e){
  c->elements.insert(e);
}

std::shared_ptr<Set>sets::unionSet(std::shared_ptr<Set>const&a,std::shared_ptr<Set>const&b){
  std::make_shared<Set>
}


class sets::EmptySet: public Set{
  public:
    EmptySet():Set(SetKind::EMPTY){}
};

class sets::CommonSet: public Set{
  public:
    CommonSet():Set(SetKind::COMMON){}
};

int main(){
  return 0;
}
