#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<stack>
#include<list>
#include<cstring>
#include<cassert>
#include<algorithm>
#include<sstream>
#include<glm/glm.hpp>
#include<geCore/Text.h>
#include<geCore/ErrorPrinter.h>

#include<geCore/Interval.h>
#include<geUtil/ArgumentFormat.h>

class ArgumentViewer{
  public:
    ArgumentViewer(int argc,char*argv[]);
    ArgumentViewer(std::vector<std::string>const&args = {});
    ~ArgumentViewer();
    size_t getNofArguments()const;
    std::string getApplicationName()const;
    std::string getArgument(size_t const&index)const;
    std::shared_ptr<ArgumentViewer>getContext(std::string const&name);
    bool                    isPresent(std::string const&argument                                           )const;
    float                   getf32   (std::string const&argument   ,float                   const&def = 0.f)const;
    double                  getf64   (std::string const&argument   ,double                  const&def = 0. )const;
    int32_t                 geti32   (std::string const&argument   ,int32_t                 const&def = 0  )const;
    int64_t                 geti64   (std::string const&argument   ,int64_t                 const&def = 0  )const;
    uint32_t                getu32   (std::string const&argument   ,uint32_t                const&def = 0  )const;
    uint64_t                getu64   (std::string const&argument   ,uint64_t                const&def = 0  )const;
    std::string             gets     (std::string const&argument   ,std::string             const&def = "" )const;
    std::vector<float   >   getf32v  (std::string const&argument   ,std::vector<float   >   const&def = {} )const;
    std::vector<double  >   getf64v  (std::string const&argument   ,std::vector<double  >   const&def = {} )const;
    std::vector<int32_t >   geti32v  (std::string const&argument   ,std::vector<int32_t >   const&def = {} )const;
    std::vector<int64_t >   geti64v  (std::string const&argument   ,std::vector<int64_t >   const&def = {} )const;
    std::vector<uint32_t>   getu32v  (std::string const&argument   ,std::vector<uint32_t>   const&def = {} )const;
    std::vector<uint64_t>   getu64v  (std::string const&argument   ,std::vector<uint64_t>   const&def = {} )const;
    std::vector<std::string>getsv    (std::string const&contextName,std::vector<std::string>const&def = {} )const;
    bool validate()const;
  protected:
    ArgumentViewer(ArgumentViewer*parent,std::vector<std::string>const&args = {});
    ArgumentViewer*_parent = nullptr;
    std::shared_ptr<ContextFormat>_format = nullptr;
    std::string _getNextArgument(std::string const&argument)const;
    size_t _getNofNextArguments(std::string const&argument)const;
    size_t _getNextArgumentIndex(std::string const&argument)const;
    std::vector<std::string>_rawArgs;
};

ArgumentViewer::ArgumentViewer(ArgumentViewer*parent,std::vector<std::string>const&args):ArgumentViewer(args){
  this->_parent = parent;
}

bool ArgumentViewer::validate()const{
  assert(this!=nullptr);
  return true;
}

ArgumentViewer::ArgumentViewer(int argc,char*argv[]){
  assert(this);
  for(int i=0;i<argc;++i)
    this->_rawArgs.push_back(std::string(argv[i]));
  this->_format = std::make_shared<ContextFormat>();
}

ArgumentViewer::ArgumentViewer(std::vector<std::string>const&args):_rawArgs(args){}

ArgumentViewer::~ArgumentViewer(){}

size_t ArgumentViewer::getNofArguments()const{
  assert(this);
  return this->_rawArgs.size();
}

std::string ArgumentViewer::getArgument(size_t const&index)const{
  assert(this);
  assert(index<this->_rawArgs.size());
  return this->_rawArgs.at(index);
}

std::shared_ptr<ArgumentViewer>ArgumentViewer::getContext(std::string const&name){
  assert(this);
  size_t index = 0;
  for(auto const&x:this->_rawArgs)
    if(x==name)break;
    else index++;
  if(index>=this->_rawArgs.size())return std::make_shared<ArgumentViewer>();
  index++;
  if(index>=this->_rawArgs.size())return std::make_shared<ArgumentViewer>();
  if(this->_rawArgs.at(index)!="{")return std::make_shared<ArgumentViewer>();
  size_t openCount = 0;
  index++;
  size_t startIndex = index;
  for(;index<this->_rawArgs.size();++index){
    if(this->_rawArgs.at(index)=="{")openCount++;
    if(this->_rawArgs.at(index)=="}"){
      if(openCount==0)break;
      else openCount--;
    }
  }
  if(index>=this->_rawArgs.size())return std::make_shared<ArgumentViewer>();
  std::vector<std::string>args;
  for(size_t i=startIndex;i<=index;++i)
    args.push_back(this->_rawArgs.at(i));
  return std::make_shared<ArgumentViewer>(args);
}

float ArgumentViewer::getf32(std::string const&argument,float const&def)const{
  assert(this);
  auto next = this->_getNextArgument(argument);
  if(next=="")return def;
  if(!ge::core::isFloat(next))return def;
  return float(std::atof(next.c_str()));
}

std::vector<float>ArgumentViewer::getf32v(std::string const&argument,std::vector<float>const&def)const{
  assert(this);
  auto start   = this->_getNextArgumentIndex(argument);
  auto nofArgs = this->_getNofNextArguments(argument);
  std::vector<float>values;
  for(size_t i=start;i<nofArgs;++i)
    if(ge::core::isFloat(this->_rawArgs.at(i)))
      values.push_back(float(std::atof(this->_rawArgs.at(i).c_str())));
    else break;
  while(values.size()<def.size())
    values.push_back(def.at(values.size()));
  return values;
}

std::string ArgumentViewer::_getNextArgument(std::string const&argument)const{
  assert(this);
  auto nextArgIndex = this->_getNextArgumentIndex(argument);
  if(nextArgIndex == std::numeric_limits<size_t>::max())return"";
  return this->_rawArgs.at(nextArgIndex);
}

size_t ArgumentViewer::_getNofNextArguments(std::string const&argument)const{
  assert(this);
  for(size_t i=0;i<this->_rawArgs.size();++i)
    if(this->_rawArgs.at(i)==argument)return this->_rawArgs.size()-i-1;
  return 0;
}

size_t ArgumentViewer::_getNextArgumentIndex(std::string const&argument)const{
  assert(this);
  for(size_t i=0;i<this->_rawArgs.size();++i)
    if(this->_rawArgs.at(i)==argument){
      if(i+1==this->_rawArgs.size())break;
      return i+1;
    }
  return std::numeric_limits<size_t>::max();
}

int main(int argc,char*argv[]){
  auto args = std::make_shared<ArgumentViewer>(argc,argv);
  auto pos = args->getContext("light")->getf32v("--position",{0.f,.0f,.0f,1.f});

  if(Interval<int32_t>(0,10).isJoinable(Interval<int32_t>(11,13)))std::cout<<"joinable"<<std::endl;
  for(auto const&x:pos)std::cout<<x<<" ";
  std::cout<<std::endl;

  SimpleValueDomain<float>dom;
  dom.addInterval(1.f,3.f);
  dom.addInterval(10.1f,19.f,Interval<float>::RIGHT_OPEN);
  dom.addInterval(0.f);
  std::cout<<dom.toStr()<<std::endl;

  std::cout<<std::atoi("+11")<<std::endl;
  std::cout<<std::atoi("-11")<<std::endl;
  std::cout<<std::atof("-11")<<std::endl;
  std::cout<<std::atof("+11.11e+11f")<<std::endl;
  return 0;
}
