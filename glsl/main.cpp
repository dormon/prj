#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include <cassert>

class Str{
  public:
    virtual std::string str(size_t i)const = 0;
};

std::ostream& operator<<(std::ostream&o,Str const&g){
  o << g.str(0);
  return o;
}

std::string indent(size_t i){
  return std::string(' ',i);
}

class Box: public Str{
  public:
    Box(std::string const&p,std::string const&e):prolog(p),epilog(e){}
    Box(std::string const&p,std::shared_ptr<Str>const&i,std::string const&e):prolog(p),epilog(e),inner(i){}
    virtual ~Box(){}
    std::string str(size_t i)const override{
      std::stringstream ss;
      ss << indent(i+pI) << prolog;
      if(inner)ss << inner->str(i+eI);
      ss << indent(i+eI) << epilog;
      return ss.str();
    }
    size_t pI = 0;
    size_t eI = 0;
    size_t iI = 0;
    std::string prolog;
    std::string epilog;
    std::shared_ptr<Str>inner;
};

class Body: public Box{
  public:
    Body(std::shared_ptr<Str>const&i):Box("{",i,"}"){}
};

class LocalSize: public Str{
  public:
    LocalSize(size_t x,size_t y=1,size_t z=1):x(x),y(y),z(z){}
    virtual ~LocalSize(){}
    std::string str(size_t i)const override{
      std::stringstream ss;
      ss << "layout(local_size_x="<<x<<",local_size_y="<<y<<",local_size_z="<<z<<")in;" << std::endl;
      return ss.str();
    }
    size_t x,y,z;
};

class Main: public Str{
  public:
    std::string str(size_t i)const override{
      std::stringstream ss;
      ss << "void main(){" << std::endl;
      ss << "}" << std::endl;
      return ss.str();
    }
};

class GLSL: public Str{
  public:
    GLSL(size_t v):version(v){}
    std::string str(size_t i)const override{
      std::stringstream ss;
      ss << "#version " << version << std::endl;
      ss << std::endl;
      if(body)ss << body->str(i);
      return ss.str();
    }
    std::shared_ptr<Str>body;
    size_t version;
};

int main(){
  auto src = GLSL(450);
  std::cerr << src << std::endl;
  //std::cerr << src << std::endl;
  return 0;
}
