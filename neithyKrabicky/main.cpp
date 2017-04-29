#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class FBox{
  protected:
    float _output;
    std::vector<std::shared_ptr<FBox>>_inputs;
    std::vector<unsigned long long>_iUpdateTicks;
    bool _changed;
    unsigned long long _ticks;
    bool _inputChanged(unsigned i)const;
    void _processInputs();
    virtual bool _do();
  public:
    FBox(unsigned n);
    void operator()();
    float getOutput()const;
    void bindInput(unsigned i,std::shared_ptr<FBox>const&box);
    bool hasInput(unsigned i)const;
    float getInputData(unsigned i)const;
};

FBox::FBox(unsigned n){
  this->_ticks = 1;
  for(unsigned i=0;i<n;++i)
    this->_inputs.push_back(nullptr);
}

bool FBox::_inputChanged(unsigned i)const{
  return this->_inputs[i]->_changed;
}

void FBox::_processInputs(){
  for(unsigned i=0;i<this->_inputs.size();++i){
    //std::cout<<this->_inputs[i]->_ticks<<"<"<<this->_ticks<<std::endl;
    if(this->hasInput(i)&&this->_inputs[i]->_ticks<this->_ticks){
      //std::cout<<"volam operator"<<std::endl;
      (*this->_inputs[i])();
      this->_inputs[i]->_ticks = this->_ticks;
    }
  }
}

bool FBox::_do(){
  return true;
}

void FBox::operator()(){
  std::cout<<"FBox::operator()()"<<std::endl;
  this->_ticks++;
  this->_processInputs();
  this->_changed = this->_do();
}

float FBox::getOutput()const{
  return this->_output;
}

void FBox::bindInput(unsigned i,std::shared_ptr<FBox>const&box){
  this->_inputs[i]=box;
  this->_inputs[i]->_ticks = this->_ticks - 1;
}

bool FBox::hasInput(unsigned i)const{
  return this->_inputs[i]!=nullptr;
}

float FBox::getInputData(unsigned i)const{
  return this->_inputs[i]->_output;
}

class Nullary: public FBox{
  protected:
    bool _do(){
      bool r= this->_changed;
      this->_changed = false;
      return r;
    }
  public:
    Nullary(float d):FBox(0){
      this->_output = d;
    }
    void update(float d){
      std::cout<<"update nullary"<<std::endl;
      this->_output = d;
      this->_changed = true;
    }
};

class AddTen: public FBox{
  protected:
    bool _do(){
      //std::cout<<"AddTen::_do start"<<std::endl;
      if(!this->_inputChanged(0))return false;
      std::cout<<"AddTen"<<std::endl;
      this->_output = this->getInputData(0) + 10.f;
      return true;
    }
  public:
    AddTen(std::shared_ptr<FBox>const&a = nullptr):FBox(1){
      this->bindInput(0,a);
    }
};

class Add: public FBox{
  protected:
    bool _do(){
      //std::cout<<"Add::_do start"<<std::endl;
      if(!this->_inputChanged(0)&&!this->_inputChanged(1))return false;
      std::cout<<"Add"<<std::endl;
      this->_output = this->getInputData(0) + this->getInputData(1);
      return true;
    }
  public:
    Add(std::shared_ptr<FBox>const&a = nullptr,std::shared_ptr<FBox>const&b = nullptr):FBox(2){
      this->bindInput(0,a);
      this->bindInput(1,a);
    }
};

template<typename T,typename...ARGS>
inline std::shared_ptr<T>sp(ARGS...args){return std::make_shared<T>(args...);}

int main(){
  auto c0=sp<Nullary>(10);
  auto f0=sp<AddTen>(c0);
  auto f1=sp<Add>(f0,f0);
  c0->update(10);
  (*f1)();
  //(*f1)();
  std::cout<<f1->getOutput()<<std::endl;
  c0->update(12);
  (*f1)();
  std::cout<<f1->getOutput()<<std::endl;
  (*f1)();
  (*f1)();

  return 0;
}





