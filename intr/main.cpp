#include<iostream>
#include<memory>
#include<functional>
#include<type_traits>
#include<cassert>
#include<vector>
#include<map>
#include<set>
#include<tuple>

#define ___ std::cerr<<__FILE__<<": "<<__LINE__<<std::endl

using FcePtr = void(*)();

template<typename OUTPUT,typename...ARGS>
OUTPUT call(OUTPUT(*fce)(ARGS...),ARGS...args){
  return fce(args...);
}

class Resource;
class Function;

class Resource{
  friend class Function;
  public:
  Resource(void*d,std::function<void(void*)>const&dt):_data(d),_destructor(dt){}
  ~Resource(){assert(this);assert(this->_destructor);this->_destructor(this->_data);}
  template<typename T>
  operator T&(){assert(this);assert(this->_data);return *(T*)this->_data;}
  template<typename T>T const&operator=(T const&d){assert(this);assert(this->_data);*(T*)this->_data = d;return d;}
  size_t getTicks()const{assert(this);return this->_ticks;}
  void updateTicks();
  template<typename T>
  void update(T const&d){assert(this);assert(this->_data);*(T*)this->_data = d;this->updateTicks();}
  protected:
  void*_data = nullptr;
  size_t _ticks = 1;
  std::function<void(void*)>_destructor;
  std::set<Function*>_signalingTargets;
};

template<typename CLASS,typename...ARGS>
std::shared_ptr<Resource>createResource(ARGS...args){
  uint8_t*ptr = new uint8_t[sizeof(CLASS)];
  assert(ptr);
  new(ptr)CLASS(args...);
  return std::make_shared<Resource>((void*)ptr,[](void*ptr){assert(ptr);((CLASS*)ptr)->~CLASS();delete[](uint8_t*)ptr;});
}

class Function{
  friend class Resource;
  public:
  void operator()(){}
  /*
  void bindInput(size_t i,std::shared_ptr<Resource>const&res){
    assert(this);
    assert(i<this->_inputs.size());
  }*/
  protected:
  using InputData = std::tuple<std::shared_ptr<Function>,std::shared_ptr<Resource>,size_t>;
  enum InputDataParts{FUNCTION=0,RESOURCE=1,TICKS=2};
  FcePtr _impl = nullptr;
  std::vector<InputData>_inputs;
  std::set<Function*>_signalingTargets;
  std::set<Function*>_signalingSources;
  bool _dirty = true;
  void _setDirty(){
    assert(this);
    if(this->_dirty)return;
    this->_dirty = true;
    for(auto const&x:this->_signalingSources){
      assert(x);
      x->_setDirty();
    }
  }
};

void Resource::updateTicks(){assert(this);this->_ticks++;for(auto const&x:this->_signalingTargets)x->_setDirty();}

int main(){
  auto rs = createResource<int>(10);
  std::cout<<(int)*rs<<std::endl;
  std::cerr<<"ahoj"<<std::endl;
	return 0;
}
