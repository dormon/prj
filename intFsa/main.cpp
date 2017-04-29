#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

class FSA;

class FusedCallback{
  public:
    typedef void(*Fce)(FSA*,void*);
    std::vector<Fce  >fces;
    std::vector<void*>data;
    inline void operator()(FSA*fsa){
      for(unsigned i=0;i<fces.size();++i)
        if(fces[i])this->fces[i](fsa,this->data[i]);
    }
    FusedCallback(Fce fce=nullptr,void*data=nullptr){
      this->fces.push_back(fce);
      this->data.push_back(data);
    }
};

class State;

class TransitionData{
  public:
    State*state = nullptr;
    FusedCallback callback;
    TransitionData(State*s = nullptr,FusedCallback::Fce fce=nullptr,void*data=nullptr){
      this->state = s;
      this->callback = FusedCallback(fce,data);
    }
};

class State{
  protected:
    std::string _name;
    std::map<std::string,TransitionData>_transition;
  public:
    State(std::string name){
      this->_name = name;
    }
    State*next(std::string lex){
      auto ii=this->_transition.find(lex);
      if(ii==this->_transition.end())return nullptr;
      return ii->second.state;
    }
    void addTransition(std::string lex,State*n,FusedCallback::Fce fce=nullptr,void*data=nullptr){
      this->_transition[lex]=TransitionData(n,fce,data);
    }
    State*apply(FSA*fsa,std::string lex){
      auto ii=this->_transition.find(lex);
      if(ii==this->_transition.end())
        return nullptr;
      ii->second.callback(fsa);
      return ii->second.state;
    }
};

class FSA{
  protected:
    std::map<std::string,State*>_name2State;
    std::string _start;
    State*_curState=nullptr;
    State*_newState(std::string name){
      auto ii=this->_name2State.find(name);
      State*result=nullptr;
      if(ii==this->_name2State.end()){
        result = new State(name);
        this->_name2State[name] = result;
      }else result = ii->second;
      return result;
    }

  public:
    FSA(std::string start){this->_start = start;}
    template<typename...ARGS>
      FSA(std::string s,ARGS...);
    ~FSA(){
      for(auto x:this->_name2State)
        delete x.second;
    }
    void addTransition(std::string A,std::string lex,std::string B,FusedCallback::Fce fce=nullptr,void*data=nullptr){
      auto a=this->_newState(A);
      auto b=this->_newState(B);
      if(a->next(lex)!=nullptr&&a->next(lex)!=b){
        std::cerr<<"ERROR: "<<A<<" -"<<lex<<"-> "<<B<<" would create nondeterministic automata"<<std::endl;
        return;
      }
      a->addTransition(lex,b,fce,data);
    }

    bool step(std::string sym,bool reset=false){
      static bool firstRun=true;
      if(reset)firstRun=true;
      if(firstRun)
        this->_curState = this->_name2State[this->_start];

      auto ns=this->_curState->apply(this,sym);
      if(!ns)return false;
      this->_curState = ns;
      return true;
    }
};

int main(){
  FSA fsa("START");
  fsa.addTransition("START","SPACE","SPACE");
  fsa.addTransition("SPACE","SPACE","SPACE_SPACE");
  return 0;
}
