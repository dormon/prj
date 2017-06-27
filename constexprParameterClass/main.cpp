#include<iostream>
#include<map>
#include<vector>
#include<typeinfo>

/*
class Parameters{
  public:
    using ParameterId = size_t;
    enum Type{
      F32,
      F64,
      I32,
      U32,
    };
    template<typename...ARGS>
    Parameters(ARGS const&...a){
      con(a...);
    }
    void con(){}
    template<typename...ARGS>
      void con(std::string const&name,Type const&type,ARGS const&...args){
        auto const id = this->name2Id.size();
        this->name2Id[name] = id;
        this->types.push_back(type);
        con(args...);
      }
    std::string type2Name(Type const&t)const{
      if(t == F32)return "f32";
      if(t == F64)return "f64";
      if(t == I32)return "i32";
      if(t == U32)return "u32";
      return "_";
    }
    void print()const{
      for(auto const&x:this->name2Id)
        std::cout << x.first << ": " << this->type2Name(this->types.at(x.second)) << std::endl;
    }
  protected:
    std::map<std::string,ParameterId>name2Id;
    std::vector<Type>types;

};

class Harris{
  public:
    static const Parameters params;
    static void printParams(){
      params.print();
    }
};

const Parameters Harris::params = Parameters("a",Parameters::F32,"b",Parameters::I32);
*/

class ParameterBase{
  public:
    ParameterBase(std::string const&name,std::type_info const&type):name(name),type(type){}
    std::string name;
    std::type_info const&type;
};

template<typename T>
class Parameter: public ParameterBase{
  public:
    Parameter(std::string const&n,T const& def):ParameterBase(n,typeid(T)),value(def){}
    T value;
};

class Parameters{
  public:
    template<typename... ARGS>
    Parameters(ARGS const&...args){
      con(args...);
    }
  protected:
    void con(){}
    template<typename T,typename ... ARGS>
    void con(Parameter<T> const&p,ARGS const&...args){
      std::cout << p.name << ": " << p.value << std::endl;
      con(args...);
    }

};


// Harris::parameters = Parameters(Parameter<float>("threshold",0.1f),Paramter<int>("numPoints",100))
//
//
//


int main()
{
  Parameters(
      Parameter<float>("ahoj" ,10.f),
      Parameter<int  >("pocet",100 ));
  //Harris::printParams();
  return 0;
}
