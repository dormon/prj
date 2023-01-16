#include<Vars/vars.hpp>
#include<iostream>
#include<fstream>
#include<memory>

#include <catch2/catch_test_macros.hpp>

using namespace vars;
using namespace std;


SCENARIO("var allocateVar deallocateVar"){
  auto ptr = allocateVar<float>();
  deallocateVar(ptr);
}

SCENARIO("var constructors and destructors"){
  int static base = 0;
  struct CD{CD(int a){base = a;}~CD(){base = 10;}};

  base = 0;
  auto ptr = allocateVar<CD>();
  callConstructor<CD>(ptr,3);
  REQUIRE(base == 3);
  auto dst = getDestructor<CD>();
  dst(ptr);
  REQUIRE(base == 10);
  deallocateVar(ptr);
}

SCENARIO("var construct"){
  int static base = 0;
  struct CD{CD(int a){base = a;}~CD(){base = 10;}};
  base = 0;
  auto ptr = construct<CD>(3);
  REQUIRE(base == 3);
  getDestructor<CD>()(ptr);
  deallocateVar(ptr);
  REQUIRE(base == 10);
}

SCENARIO("var callConstructor"){
  int static base = 0;
  struct CD{CD(int a){base = a;}~CD(){base = 10;}};
  base = 0;
  auto ptr = allocateVar<CD>();
  callConstructor<CD>(ptr,3);
  {
    auto a = Var{ptr,getDestructor<CD>(),typeid(CD)};
    REQUIRE(base == 3);
  }
  REQUIRE(base == 10);
}

SCENARIO("var create var"){
  var<float>();
}

SCENARIO("var create var with default"){
  var<float>(1.3f);
}

SCENARIO("var create var and set value"){
    auto a = var<float>(3.f);
    a = 3.f;
}

SCENARIO("var stamps"){
  auto a = var<float>(3.f);
  REQUIRE(a.stamp() == 0);
  a.updateStamp();
  REQUIRE(a.stamp() == 1);
  a.reCreate<float>(4.f);
  REQUIRE(a.stamp() == 2);
  a.reCreate<uint16_t>(2);
  REQUIRE(a.stamp() == 3);
}

SCENARIO("var type"){
  auto a = var<float>(3.f);
  REQUIRE(a.getType() == typeid(float));
}


#if 0
SCENARIO("Simple directory"){
  Directory root;
  REQUIRE(root.getVars() == set<string>{});
  root.add("aaa");
  REQUIRE(root.vars == set<string>{"aaa"});
  root.remove("aaa");
  REQUIRE(root.getVars() == set<string>{});
  root.add("a.b");
  REQUIRE(root.vars == set<string>{});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").vars == set<string>{"b"});
  root.removeDir("a");
  REQUIRE(root.vars == set<string>{});
  REQUIRE(!root.hasDir("a"));
  root.add("a.b");
  root.add("a");
  REQUIRE(root.vars == set<string>{"a"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").vars == set<string>{"b"});
  root.remove("a");
  REQUIRE(root.getVars() == set<string>{});
  REQUIRE(!root.hasDir("a"));
  REQUIRE(!root.hasVar("a"));
}

SCENARIO("Directory"){
  Directory root;

  REQUIRE(root.getVars() == set<string>{});

  root.add("aaa");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.dirs.empty());

  root.add("aaa");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.dirs.empty());

  root.add("a.b");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").vars == set<string>{"b"});

  root.add("a.b");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").vars == set<string>{"b"});

  root.add("a.c");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").vars == set<string>{"b","c"});

  root.add("a.b.x");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  auto const&adir = root.getDir("a");
  REQUIRE(adir.vars == set<string>{"b","c"});
  REQUIRE(adir.hasDir("b"));
  REQUIRE(adir.getDir("b").vars == set<string>{"x"});

  root.add("a.e.x");
  REQUIRE(root.vars == set<string>{"aaa"});
  REQUIRE(root.hasDir("a"));
  REQUIRE(root.getDir("a").hasDir("e"));
  REQUIRE(root.getDir("a").getDir("e").hasVar("x"));

  REQUIRE(root.getVars() == set<string>{"aaa","a.b","a.c","a.b.x","a.e.x"});

  REQUIRE(root.isDir("aaa") == false);
  REQUIRE(root.isVar("aaa") == true );
  REQUIRE(root.isVar("a.b") == true );
  REQUIRE(root.isVar("a.c") == true );
  REQUIRE(root.isVar("a.b.x") == true );
  REQUIRE(root.isVar("a.e.x") == true );
  REQUIRE(root.isDir("a.b") == true);
  REQUIRE(root.isDir("a.e") == true);

  REQUIRE(root.getVars("a") == set<string>{"a.b","a.c","a.b.x","a.e.x"});
  REQUIRE(root.getVars("a.b") == set<string>{"a.b.x"});
  REQUIRE(root.getVars("a.e") == set<string>{"a.e.x"});
  REQUIRE(root.getVars("a.e.x") == set<string>{});

  root.remove("aaa");
  REQUIRE(root.getVars() == set<string>{"a.b","a.c","a.b.x","a.e.x"});
  root.remove("a.b");
  REQUIRE(root.getVars() == set<string>{"a.c","a.e.x"});
  root.remove("a");
  REQUIRE(root.getVars() == set<string>{});
}

SCENARIO("Directory II"){
  Directory root;
  root.add("a");
  root.add("a.a");
  REQUIRE(root.isDir("a") == true);
  REQUIRE(root.isVar("a") == true);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == true);

  root.removeVar("a");
  REQUIRE(root.isDir("a") == true);
  REQUIRE(root.isVar("a") == false);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == true);

  root.removeVar("a.a");
  REQUIRE(root.isDir("a") == false);
  REQUIRE(root.isVar("a") == false);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == false);

  root.add("a");
  root.add("a.a");
  REQUIRE(root.isDir("a") == true);
  REQUIRE(root.isVar("a") == true);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == true);
  root.remove("a");
  REQUIRE(root.isDir("a") == false);
  REQUIRE(root.isVar("a") == false);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == false);

  root.add("a");
  root.add("a.a");
  REQUIRE(root.isDir("a") == true);
  REQUIRE(root.isVar("a") == true);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == true);
  root.remove("a.a");
  REQUIRE(root.isDir("a") == false);
  REQUIRE(root.isVar("a") == true);
  REQUIRE(root.isDir("a.a") == false);
  REQUIRE(root.isVar("a.a") == false);
}
#endif

SCENARIO("Vars::has tests"){
  Vars vars;
  REQUIRE(vars.has("checkBox") == false);
}

SCENARIO("Vars::br test"){
  Vars vars;
  vars.br("checkBox",true);
  REQUIRE(vars.has("checkBox") == true);
}

SCENARIO("Vars::remove test"){
  Vars vars;
  vars.br("checkBox",true);
  REQUIRE(vars.has("checkBox") == true);
  vars.remove("checkBox");
  REQUIRE(vars.has("checkBox") == false);
}

SCENARIO("Vars::remove test2"){
  Vars vars;
  vars.br("checkBox",true);
  REQUIRE(vars.has("checkBox") == true);
  vars.remove("checkBox");
  REQUIRE(vars.has("checkBox") == false);
  vars.br("checkBox",true);
  REQUIRE(vars.has("checkBox") == true);
  vars.remove("checkBox");
  REQUIRE(vars.has("checkBox") == false);
}

SCENARIO("Vars::get test"){
  Vars vars;
  vars.br("checkBox",true);
  auto v = vars.get("checkBox");
  REQUIRE(v != nullptr);
  REQUIRE(v->getType() == typeid(bool));
}

//SCENARIO("Vars::getType test2"){
//  Vars vars;
//  vars.br("checkBox",true);
//  REQUIRE(vars.getType("checkBox") == typeid(bool));
//}

//SCENARIO("Basic vars tests"){
//  Vars vars;
//  REQUIRE(vars.has("checkBox") == false);
//  vars.br("checkBox",true);
//  REQUIRE(vars.has("checkBox") == true);
//  REQUIRE(vars.getType("checkBox") == typeid(bool));
//  REQUIRE(vars.b("checkBox")==true);
//  REQUIRE(vars.stamp("checkBox")==1);
//  //vars.b("checkBox") = false;
//  //vars.updateStamp("checkBox");
//  //REQUIRE(vars.b("checkBox")==false);
//  //REQUIRE(vars.stamp("checkBox")==2);
//  //vars.remove("checkBox");
//  //REQUIRE(vars.has("checkBox") == false);
//}


#if 0
SCENARIO("Vars vs Dirs"){
  Vars vars;
  vars.addBool("a");
  vars.addBool("a.a");

  REQUIRE(vars.isDir("a") == true);
  REQUIRE(vars.isVar("a") == true);
  REQUIRE(vars.isVar("a.a") == true);
  REQUIRE(vars.isDir("a.a") == false);
  REQUIRE(vars.getKind("a") == ResourceKind::BASIC);
  REQUIRE(vars.getKind("a.a") == ResourceKind::BASIC);
}

SCENARIO("Erase vars tests"){
  Vars vars;
  vars.addBool("a.a.a");
  vars.addBool("a.a.b");
  vars.addBool("a.a.c");
  vars.addBool("a.b.a");
  vars.addBool("a.b.b");
  vars.addBool("a.b.c");
  vars.addBool("a.c.a");
  vars.addBool("a.c.b");
  vars.addBool("a.c.c");
  vars.addBool("a.a");
  vars.addBool("a.b");
  vars.addBool("a.c");
  vars.addBool("a");
  vars.addBool("b");
  vars.addBool("c");
  REQUIRE(vars.isDir("a.a.a") == false);
  REQUIRE(vars.isDir("a.a.b") == false);
  REQUIRE(vars.isDir("a.a.c") == false);
  REQUIRE(vars.isDir("a.a.a") == false);
  REQUIRE(vars.isDir("a.a.b") == false);
  REQUIRE(vars.isDir("a.a.c") == false);
  REQUIRE(vars.isDir("a.a") == true);
  REQUIRE(vars.isDir("a.b") == true);
  REQUIRE(vars.isDir("a.c") == true);
  REQUIRE(vars.isDir("a") == true);
  REQUIRE(vars.isDir("b") == false);
  REQUIRE(vars.isDir("c") == false);

  REQUIRE(vars.isVar("a.a.a") == true);
  REQUIRE(vars.isVar("a.a.b") == true);
  REQUIRE(vars.isVar("a.a.c") == true);
  REQUIRE(vars.isVar("a.a.a") == true);
  REQUIRE(vars.isVar("a.a.b") == true);
  REQUIRE(vars.isVar("a.a.c") == true);
  REQUIRE(vars.isVar("a.a") == true);
  REQUIRE(vars.isVar("a.b") == true);
  REQUIRE(vars.isVar("a.c") == true);
  REQUIRE(vars.isVar("a") == true);
  REQUIRE(vars.isVar("b") == true);
  REQUIRE(vars.isVar("c") == true);

  vars.eraseDir("a");

  REQUIRE(vars.isDir("a.a.a") == false);
  REQUIRE(vars.isDir("a.a.b") == false);
  REQUIRE(vars.isDir("a.a.c") == false);
  REQUIRE(vars.isDir("a.a.a") == false);
  REQUIRE(vars.isDir("a.a.b") == false);
  REQUIRE(vars.isDir("a.a.c") == false);
  REQUIRE(vars.isDir("a.a") == false);
  REQUIRE(vars.isDir("a.b") == false);
  REQUIRE(vars.isDir("a.c") == false);
  REQUIRE(vars.isDir("a") == false);
  REQUIRE(vars.isDir("b") == false);
  REQUIRE(vars.isDir("c") == false);

  REQUIRE(vars.isVar("a.a.a") == false);
  REQUIRE(vars.isVar("a.a.b") == false);
  REQUIRE(vars.isVar("a.a.c") == false);
  REQUIRE(vars.isVar("a.a.a") == false);
  REQUIRE(vars.isVar("a.a.b") == false);
  REQUIRE(vars.isVar("a.a.c") == false);
  REQUIRE(vars.isVar("a.a") == false);
  REQUIRE(vars.isVar("a.b") == false);
  REQUIRE(vars.isVar("a.c") == false);
  REQUIRE(vars.isVar("a") == true);
  REQUIRE(vars.isVar("b") == true);
  REQUIRE(vars.isVar("c") == true);

  vars.eraseVar("a");
  vars.eraseVar("b");
  vars.eraseVar("c");
  REQUIRE(vars.isVar("a") == false);
  REQUIRE(vars.isVar("b") == false);
  REQUIRE(vars.isVar("c") == false);

  vars.addBool("a.a");
  vars.addBool("a.b");
  vars.addBool("a.c");
  vars.addBool("a");
  vars.erase("a");

  REQUIRE(vars.isVar("a.a") == false);
  REQUIRE(vars.isVar("a.b") == false);
  REQUIRE(vars.isVar("a.c") == false);
  REQUIRE(vars.isVar("a") == false);
}

SCENARIO("Recreate tests"){
  Vars vars;
  vars.addBool("var",false);
  REQUIRE(vars.getBool("var") == false);
  REQUIRE(vars.getKind("var") == ResourceKind::BASIC);
  vars.reCreate<float>("var",1.3f);
  REQUIRE(vars.getTicks("var") == 2);
  REQUIRE(vars.getFloat("var") == 1.3f);
  REQUIRE(vars.getKind("var") == ResourceKind::BASIC);
}

SCENARIO("Recreate tests, return value"){
  Vars vars;
  auto ptr = vars.reCreate("a",(void*)17,[](void*){},typeid(float),ResourceKind::BASIC);
  REQUIRE(ptr == (void*)17);
  REQUIRE(vars.getKind("a") == ResourceKind::BASIC);
  ptr = vars.reCreate("a",(void*)9223,[](void*){},typeid(float),ResourceKind::BASIC);
  REQUIRE(vars.getKind("a") == ResourceKind::BASIC);
  REQUIRE(ptr == (void*)9223);
}

class Object{
  public:
    Object(size_t n){
      d = new float[n];
    }
    ~Object(){
      delete[]d;
    }
  protected:
    float*d = nullptr;
};

SCENARIO("Custom object"){
  Vars vars;
  vars.add<Object>("obj",10);
  REQUIRE(vars.getKind("obj") == ResourceKind::CLASS);
  vars.erase("obj");
}

class Base{
  public:
    Base(Vars&vars):vars(vars){}
    virtual ~Base(){}
    Vars&vars;
};

class Child: public Base{
  public:
    Child(Vars&vars):Base(vars){
      vars.addBool("asdasd",false);
    }
    virtual ~Child(){
      vars.erase("asdasd");
    }
};

SCENARIO("Vars in child object"){
  Vars vars;
  REQUIRE(vars.has("asdasd") == false);
  auto c = make_unique<Child>(vars);
  REQUIRE(vars.has("asdasd") == true);
  c = nullptr;
  REQUIRE(vars.has("asdasd") == false);
}

SCENARIO("Vars in child object, child object in vars"){
  Vars vars;
  REQUIRE(vars.has("asdasd") == false);
  vars.add<Child>("child",vars);
  REQUIRE(vars.has("asdasd") == true);
  REQUIRE(vars.has("child") == true);
  vars.erase("child");
  REQUIRE(vars.has("asdasd") == false);
  REQUIRE(vars.has("child") == false);
}

SCENARIO("Vars in child object, child object in vars, default destructor"){
  Vars vars;
  REQUIRE(vars.has("asdasd") == false);
  vars.add<Child>("child",vars);
  REQUIRE(vars.has("asdasd") == true);
  REQUIRE(vars.has("child") == true);
}



SCENARIO("Vars test for non existing variable"){
  Vars vars;
  bool catchedError = false;
  try{
    vars.getBool("hi");
  }catch(std::runtime_error&e){
    catchedError = true;
  }
  REQUIRE(catchedError == true);

  catchedError = false;
  try{
    vars.get<Object>("hi");
  }catch(std::runtime_error&e){
    catchedError = true;
  }
  REQUIRE(catchedError == true);

}

struct Vec3{
  Vec3(float d){a[0]=a[1]=a[2]=d;}
  float a[3];
};

SCENARIO("Vars - vector test"){
  Vars vars;
  auto&v=vars.addVector<float>("a");
  REQUIRE(vars.has("a") == true);
  v.push_back(1.f);
  v.push_back(2.f);
  auto&w=vars.reCreateVector<float>("a",10);
  REQUIRE(w.size() == 10);
  vars.erase("a");
  REQUIRE(vars.has("a") == false);

  auto&vv=vars.addVector<Vec3>("a");
  vv.resize(10,Vec3(10.f));
  REQUIRE(vv.at(9).a[0] == 10.f);
  REQUIRE(vv.at(9).a[1] == 10.f);
  REQUIRE(vv.at(9).a[2] == 10.f);
}

bool exceptionCatcher(std::function<void()>const&f){
  try{
    f();
  }catch(std::runtime_error&e){
    return true;
  }
  return false;
}

SCENARIO("Vars - getVarName"){
  Vars vars;
  REQUIRE(exceptionCatcher([&](){vars.getVarName(0);}) == true);
  vars.addBool("a");
  REQUIRE(exceptionCatcher([&](){vars.getVarName(0);}) == false);
  REQUIRE(vars.getVarName(0) == "a");
  vars.addBool("b");
  REQUIRE(vars.getVarName(1) == "b");
  vars.addFloat("c");
  REQUIRE(vars.getVarName(2) == "c");
  vars.addUint32("b.a");
  REQUIRE(vars.getVarName(3) == "b.a");
  vars.erase("b");
  REQUIRE(vars.getVarName(0) == "a");
  REQUIRE(vars.getVarName(1) == "c");
  REQUIRE(exceptionCatcher([&](){vars.getVarName(2);}) == true);
}

SCENARIO("Vars - getNofVars"){
  Vars vars;
  REQUIRE(vars.getNofVars() == 0);
  vars.addBool("a");
  REQUIRE(vars.getNofVars() == 1);
  vars.addBool("b");
  REQUIRE(vars.getNofVars() == 2);
  vars.addFloat("c");
  REQUIRE(vars.getNofVars() == 3);
  vars.erase("a");
  REQUIRE(vars.getNofVars() == 2);
  vars.addUint32("b.a");
  REQUIRE(vars.getNofVars() == 3);
  vars.erase("b");
  REQUIRE(vars.getNofVars() == 1);
}

SCENARIO("Vars - addOrGet"){
  Vars vars;
  size_t constructorCalled = 0;
  size_t destructorCalled = 0;
  class A{
    public:
      A(size_t*cc,size_t*dd):c(cc),d(dd){(*c)++;}
      ~A(){(*d)++;}
      size_t*c;
      size_t*d;
  };
  vars.addOrGet<A>("a",&constructorCalled,&destructorCalled);
  REQUIRE(constructorCalled == 1);
  REQUIRE(destructorCalled == 0);
  vars.addOrGet<A>("a",&constructorCalled,&destructorCalled);
  REQUIRE(constructorCalled == 1);
  REQUIRE(destructorCalled == 0);

  vars.addOrGetBool("check",true);
  REQUIRE(vars.getBool("check") == true);
  vars.addOrGetBool("check",false);
  REQUIRE(vars.getBool("check") == true);
}

SCENARIO("Vars - enum"){
  Vars vars;
  enum E{
    E_A,
    E_B,
    E_C,
  };
  vars.addEnum<E>("e",E_A);
  auto&v = vars.getEnum<E>("e");
  REQUIRE(v == E_A);
  REQUIRE(vars.getKind("e") == ResourceKind::ENUM);
  vars.reCreate<E>("e",E_B);
  auto&w = vars.getEnum<E>("e");
  REQUIRE(w == E_B);
  REQUIRE(vars.getKind("e") == ResourceKind::ENUM);
}

std::set<std::string>vector2Set(std::vector<std::string>const&vv){
  std::set<std::string>rr;
  for(auto const&n:vv)
    rr.insert(n);
  return rr;
}

SCENARIO("Vars - getDir"){
  Vars vars;
  vars.addBool("a");
  vars.addBool("dir.a");
  vars.addBool("dir.b");
  vars.addBool("dir.dir.a");
  vars.addBool("dir.dir.b");
  std::vector<std::string>fullNames;
  std::vector<std::string>names;
  std::set<std::string>sFullNames;
  std::set<std::string>sNames;

  vars.getDirFullNames(fullNames,"dir");
  sFullNames = vector2Set(fullNames);
  vars.getDir(names,"dir");
  sNames = vector2Set(names);
  REQUIRE(sFullNames == std::set<std::string>{"dir.a","dir.b","dir.dir.a","dir.dir.b"});
  REQUIRE(sNames == std::set<std::string>{"a","b","dir.a","dir.b"});

  vars.getDirFullNames(fullNames,"dir.dir");
  sFullNames = vector2Set(fullNames);
  vars.getDir(names,"dir.dir");
  sNames = vector2Set(names);
  REQUIRE(sFullNames == std::set<std::string>{"dir.dir.a","dir.dir.b"});
  REQUIRE(sNames == std::set<std::string>{"a","b"});

  vars.erase("dir");
  vars.getDirFullNames(fullNames,"dir");
  sFullNames = vector2Set(fullNames);
  vars.getDir(names,"dir");
  sNames = vector2Set(names);
  REQUIRE(sFullNames == std::set<std::string>{});
  REQUIRE(sNames == std::set<std::string>{});
}

SCENARIO("Vars - is"){
  Vars vars;
  vars.addBool("a");
  vars.addInt16("dir.a");
  vars.addFloat("dir.b");
  vars.addString("dir.dir.a");

  struct A{float a;};
  vars.add<A>("dir.dir.b");

  REQUIRE(vars.is<bool>("a") == true);
  REQUIRE(vars.is<int16_t>("dir.a") == true);
  REQUIRE(vars.is<float >("dir.b") == true);
  REQUIRE(vars.is<std::string>("dir.dir.a") == true);
  REQUIRE(vars.is<A>("dir.dir.b") == true);

  REQUIRE(vars.isBool("a") == true);
  REQUIRE(vars.isInt16("dir.a") == true);
  REQUIRE(vars.isFloat("dir.b") == true);
  REQUIRE(vars.isString("dir.dir.a") == true);
}
#endif
