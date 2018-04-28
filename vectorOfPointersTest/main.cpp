#include <iostream>
#include <tuple>
#include <sstream>
#include <vector>
#include <memory>
#include <chrono>
#include <functional>
#include <iomanip>
#include <map>

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


template<typename T>
T measure(std::function<void()>const&f){
  auto startTime = std::chrono::high_resolution_clock::now();
  f();
  auto endTime   = std::chrono::high_resolution_clock::now();
  std::chrono::duration<T>const elapsed = endTime - startTime;
  return elapsed.count();
}

int main() {
  size_t N = 10000000;
  std::cout << "class Base{...};" << std::endl;
  std::cout << "class Child: public Base{...};" << std::endl;
  std::cout << "inserting N = " << N << " polymorphic elements to vector" << std::endl;
  std::cout << std::fixed;
  std::cout << std::setprecision(5);
  {
    std::vector<std::unique_ptr<Option>>opts;
    auto const what = [&](){
      for(size_t i=0;i<N;++i)
        opts.push_back(std::make_unique<Atomic<float>>("mass",1.f));
    };
    std::cout << "vector<unique_ptr<Base>>::push_back: " << measure<float>(what) << " s" << std::endl;
    auto const clr = [&](){
      opts.clear();
    };
    std::cout << "vector<unique_ptr<Base>>::clear    : " << measure<float>(clr) << " s" << std::endl;
  }
  {
    std::vector<std::shared_ptr<Option>>opts;
    auto const what = [&](){
      for(size_t i=0;i<N;++i)
        opts.push_back(std::make_shared<Atomic<float>>("mass",1.f));
    };
    std::cout << "vector<shared_ptr<Base>>::push_back: " << measure<float>(what) << " s" << std::endl;
    auto const clr = [&](){
      opts.clear();
    };
    std::cout << "vector<shared_ptr<Base>>::clear    : " << measure<float>(clr) << " s" << std::endl;
  }
  {
    std::vector<Option*>opts;
    auto const what = [&](){
      for(size_t i=0;i<N;++i)
        opts.push_back(new Atomic<float>("mass",1.f));
    };
    std::cout << "vector<Base*>::push_back           : " << measure<float>(what) << " s" << std::endl;
    auto const clr = [&](){
      for(auto const&x:opts)delete x;
      opts.clear();
    };
    std::cout << "vector<Base*>::clear               : " << measure<float>(clr) << " s" << std::endl;
  }

  {
    std::vector<Atomic<float>>opts;
    auto const what = [&](){
      for(size_t i=0;i<N;++i)
        opts.push_back(Atomic<float>("mass",1.f));
    };
    std::cout << "vector<Child>::push_back           : " << measure<float>(what) << " s" << std::endl;
    auto const clr = [&](){
      opts.clear();
    };
    std::cout << "vector<Child>::clear               : " << measure<float>(clr) << " s" << std::endl;
  }

  {
    std::vector<Atomic<float>>opts;
    auto const what = [&](){
      for(size_t i=0;i<N;++i)
        opts.emplace_back("mass",1.f);
    };
    std::cout << "vector<Child>::emplace_back        : " << measure<float>(what) << " s" << std::endl;
    auto const clr = [&](){
      opts.clear();
    };
    std::cout << "vector<Child>::clear               : " << measure<float>(clr) << " s" << std::endl;
  }

  return 0;
}
