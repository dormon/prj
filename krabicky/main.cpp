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
    std::vector<bool>_changed;
    unsigned long long _updateTicks;
    unsigned long long _checkTicks;
    bool _inputChanged(unsigned i)const;
    void _processInputs();
    virtual bool _do();
  public:
    FBox(unsigned n);
    void operator()();
    void setCheckTicks(unsigned long long);
    float getOutput()const;
    void bindInput(unsigned i,std::shared_ptr<FBox>const&box);
    bool hasInput(unsigned i)const;
    float getInputData(unsigned i)const;
};

FBox::FBox(unsigned n){
  this->_updateTicks = 1;
  this->_checkTicks = 0;
  for(unsigned i=0;i<n;++i){
    this->_inputs.push_back(nullptr);
    this->_iUpdateTicks.push_back(0);
    this->_changed.push_back(false);
  }
}

bool FBox::_inputChanged(unsigned i)const{
  return this->_changed[i];
}

void FBox::_processInputs(){
  for(unsigned i=0;i<this->_inputs.size();++i){
    if(!this->hasInput(i)||this->_inputs[i]->_checkTicks>=this->_checkTicks){
      this->_changed[i]=false;
      continue;
    }
    (*this->_inputs[i])();
    this->_inputs[i]->_checkTicks = this->_checkTicks;
    this->_changed[i]=this->_iUpdateTicks[i]<this->_inputs[i]->_updateTicks;
    if(this->_changed[i])this->_iUpdateTicks[i]=this->_inputs[i]->_updateTicks;
  }
}

bool FBox::_do(){
  return true;
}

void FBox::operator()(){
  std::cout<<"FBox::operator()()"<<std::endl;
  this->_checkTicks++;
  this->_processInputs();
  if(this->_do())
    this->_updateTicks++;
}

float FBox::getOutput()const{
  return this->_output;
}

void FBox::bindInput(unsigned i,std::shared_ptr<FBox>const&box){
  this->_inputs[i]=box;
  this->_iUpdateTicks[i]=box->_updateTicks-1;
  this->_changed[i]=true;
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
      return false;
    }
  public:
    Nullary(float d):FBox(0){
      this->_output = d;
    }
    void update(float d){
      std::cout<<"update nullary"<<std::endl;
      this->_output = d;
      this->_updateTicks++;
    }
};

class AddTen: public FBox{
  protected:
    bool _do(){
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





