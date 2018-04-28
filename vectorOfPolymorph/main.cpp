#include <iostream>
#include <tuple>
#include <sstream>
#include <vector>
#include <memory>

class Option{
  public:
    virtual ~Option(){}
    Option(std::string const&n):name(n){}
    std::string getName()const{return name;}
  protected:
    std::string name;
};

template<typename T>
class Atomic: public Option{
  public:
    Atomic(std::string const&n,T const&d):Option(n),data(d){}
    T getData()const{return data;}
  protected:
    T data;
};

void print(std::vector<std::unique_ptr<Option>>const&opts){
  for(auto const&o:opts)
    std::cout << o->getName() << std::endl;
}

int main() {
  std::vector<std::unique_ptr<Option>>opts;
  opts.push_back(std::make_unique<Atomic<float>>("mass",1.f));
  opts.push_back(std::make_unique<Atomic<size_t>>("count",100));
  opts.push_back(std::make_unique<Atomic<std::string>>("fileName","/usr/data"));
  print(opts);
  std::cout << dynamic_cast<Atomic<std::string>*>(opts.at(2).get())->getData() << std::endl;
  //opts.push_back(std::reference_wrapper<AtomicOption<float>>("mass",1.f));
  //opts.push_back(AtomicOption<size_t>("count",100));

  //std::cout << dynamic_cast<AtomicOption<float>*>(&opts.at(0))->getData() << std::endl;
  return 0;
}
