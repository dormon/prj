#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>
#include<cassert>
#include<glm/glm.hpp>

class AddNeuron;
class VarNeuron;
class Neuron{
  public:
    enum Type{VARNEURON,ADDNEURON}type;
    Neuron(Type const&t,double val = 0.f):type(t),_output(val){}
    virtual ~Neuron(){}
    virtual double operator()() = 0;
    virtual void addInput(double weigth,std::shared_ptr<Neuron>const&neuron) = 0;
    virtual void removeInput(std::shared_ptr<Neuron>const&neuron) = 0;
    double getOutput()const{
      assert(this);
      return this->_output;
    }
  protected:
    std::set<Neuron*>_signalingSources;
    std::set<Neuron*>_signalingTargets;
    bool _dirty = true;
    virtual void _setDirty(){
      if(this->_dirty)return;
      assert(this!=nullptr);
      this->_dirty = true;
      for(auto const&x:this->_signalingTargets){
        assert(x);
        x->_setDirty();
      }
    }
    double _output = 0.f;
  friend class AddNeuron;
  friend class VarNeuron;
};

double sigmoid(double x){
  return 2.f/(1.f+glm::exp(-x))-1.f;
}

class AddNeuron: public Neuron{
  public:
    AddNeuron(double val = 0.f):Neuron(Neuron::ADDNEURON,val){}
    virtual ~AddNeuron(){
      assert(this);
      for(auto const&x:this->_signalingSources)
        x->_signalingTargets.erase(this);
      for(auto const&x:this->_signalingTargets)
        x->_signalingSources.erase(this);
    }
    double operator()()override{
      assert(this);
      if(!this->_dirty)return this->_output;
      this->_dirty = false;
      assert(this);
      this->_output=false;
      for(auto const&x:this->_connections){
        assert(x.first);
        this->_output += x.second*(*x.first)();
      }
      //this->_output = sigmoid(this->_output);
      this->_setDirty();
      return this->_output;
    }
    virtual void addInput(double weigth,std::shared_ptr<Neuron>const&neuron)override{
      assert(this);
      assert(neuron);
      this->_connections[neuron] = weigth;
      neuron->_signalingTargets.insert(this);
      this->_signalingSources.insert(&*neuron);
      this->_dirty = true;
    }
    virtual void removeInput(std::shared_ptr<Neuron>const&neuron)override{
      assert(this);
      this->_connections.erase(neuron);
      neuron->_signalingTargets.erase(this);
      this->_signalingSources.erase(&*neuron);
      this->_dirty = true;
    }
  protected:
    std::map<std::shared_ptr<Neuron>,double>_connections;
};

class VarNeuron: public Neuron{
  public:
    VarNeuron(double val = 0.f):Neuron(Neuron::VARNEURON,val){}
    virtual ~VarNeuron(){
      if(this->_producer){
        this->_producer->_signalingTargets.erase(this);
        this->_signalingSources.erase(this->_producer);
      }
    }
    void update(double val){
      assert(this);
      this->_dirty = false;
      this->_output = val;
      this->_setDirty();
    }
    virtual double operator()()override{
      assert(this);
      return this->_output;
    }
    virtual void addInput(double weight,std::shared_ptr<Neuron>const&neuron)override{
      assert(this);
      assert(neuron);
      this->_producerWeight = weight;
      this->_producer = &*neuron;
      neuron->_signalingTargets.insert(this);
      this->_signalingSources.insert(&*neuron);
    }
    virtual void removeInput(std::shared_ptr<Neuron>const&neuron)override{
      assert(this);
      assert(neuron);
      if(&*neuron == this->_producer){
        this->_signalingSources.erase(&*neuron);
        neuron->_signalingTargets.erase(this);
        this->_producer = nullptr;
      }
    }
  protected:
    virtual void _setDirty()override{
     assert(this);
      if(this->_producer){
        this->_dirty = false;
        this->_output = this->_producer->_output;
        for(auto const&x:this->_signalingTargets)
          x->_setDirty();
      }
    }
    double _producerWeight = 0.f;
    Neuron*_producer = nullptr;
};

class Network{
  public:
    Network(size_t nofInputs,size_t nofOutputs){
      for(size_t i=0;i<nofInputs;++i)
        this->_inputs.push_back(std::make_shared<VarNeuron>());
      for(size_t i=0;i<nofOutputs;++i)
        this->_inputs.push_back(std::make_shared<VarNeuron>());
    }
    void setInputs(std::vector<double>const&in){
      assert(this);
      assert(this->_inputs.size()==in.size());
      for(size_t i=0;i<in.size();++i){
        assert(std::dynamic_pointer_cast<VarNeuron>(this->_inputs.at(i)));
        std::dynamic_pointer_cast<VarNeuron>(this->_inputs.at(i))->update(in.at(i));
      }
    }
    void operator()(){
      assert(this);
      for(auto const&x:this->_outputs)
        (*x)();
    }
    void getOutput(std::vector<double>&out)const{
      assert(this);
      for(auto const&x:this->_outputs){
        assert(std::dynamic_pointer_cast<VarNeuron>(x));
        out.push_back(std::dynamic_pointer_cast<VarNeuron>(x)->getOutput());
      }
    }
  protected:
    std::vector<std::shared_ptr<Neuron>>_inputs;
    std::vector<std::shared_ptr<Neuron>>_outputs;
    std::vector<std::shared_ptr<Neuron>>_hidden;
};

int main(){
  std::shared_ptr<Neuron> acc = std::make_shared<VarNeuron>(0.f);
  std::shared_ptr<Neuron> one = std::make_shared<VarNeuron>(1.f);
  std::shared_ptr<Neuron> add = std::make_shared<AddNeuron>(0.f);
  add->addInput(1.f,acc);
  add->addInput(1.f,one);
  acc->addInput(1.f,add);

  std::cout<<add->getOutput()<<std::endl;(*add)();
  std::cout<<add->getOutput()<<std::endl;(*add)();
  std::cout<<add->getOutput()<<std::endl;(*add)();
  std::cout<<add->getOutput()<<std::endl;(*add)();
  std::cout<<add->getOutput()<<std::endl;(*add)();
  return 0;
}
