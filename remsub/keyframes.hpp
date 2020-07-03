#pragma once
#include<glm/glm.hpp>
#include<iostream>
#include<map>

namespace keyframe{

class Data{
  public:
    enum Type{
      INT   ,
      FLOAT ,
      DOUBLE,
      VEC4  ,
    }type;
    union Value{
      int    i32;
      float  f32;
      double f64;
      glm::vec4 vec4;
      Value(float           v):f32 (v){}
      Value(int             v):i32 (v){}
      Value(double          v):f64 (v){}
      Value(glm::vec4 const&v):vec4(v){}
    }value;
    void set(int             v){value = v;}
    void set(float           v){value = v;}
    void set(double          v){value = v;}
    void set(glm::vec4 const&v){value = v;}
    Data(float           v):type(FLOAT ),value(v){}
    Data(int             v):type(INT   ),value(v){}
    Data(double          v):type(DOUBLE),value(v){}
    Data(glm::vec4 const&v):type(VEC4  ),value(v){}
    template<typename T>T get()const;
    template<typename T>T&get();
    void print()const{
      if(type == INT   )std::cout << value.i32;
      if(type == FLOAT )std::cout << value.f32;
      if(type == DOUBLE)std::cout << value.f64;
      if(type == VEC4  )std::cerr << "vec4("<<value.vec4.x<<","<<value.vec4.y<<","<<value.vec4.z<<","<<value.vec4.w<<")";
    }
};
template<>int       Data::get<int      >()const{return value.i32; }
template<>float     Data::get<float    >()const{return value.f32; }
template<>double    Data::get<double   >()const{return value.f64; }
template<>glm::vec4 Data::get<glm::vec4>()const{return value.vec4;}
template<>int      &Data::get<int      >()     {return value.i32; }
template<>float    &Data::get<float    >()     {return value.f32; }
template<>double   &Data::get<double   >()     {return value.f64; }
template<>glm::vec4&Data::get<glm::vec4>()     {return value.vec4;}


class Keyframe{
  public:
    std::map<std::string,Data>data;
    Keyframe(std::string const&n,int             v){set(n,v);}
    Keyframe(std::string const&n,float           v){set(n,v);}
    Keyframe(std::string const&n,double          v){set(n,v);}
    Keyframe(std::string const&n,glm::vec4 const&v){set(n,v);}

    template<typename T>
    void set(std::string const&n,T const&v){
      auto w = data.emplace(n,v);
      if(!std::get<1>(w)){
        std::get<0>(w)->second.set(v);
      }
    }
    template<typename T>
    T get(std::string const&n,T v)const{
      auto it = data.find(n);
      if(it == std::end(data))return v;
      return it->second.get<T>();
    }
    template<typename T>
    T&get(std::string const&n,T&v){
      auto it = data.find(n);
      if(it == std::end(data))return v;//throw std::runtime_error("Keyframe::get("+n+") - no such value");
      return it->second.get<T>();
    }
    bool has(std::string const&n)const{
      return data.count(n) != 0;
    }
    void erase(std::string const&n){
      data.erase(n);
    }
    void print(int indent)const{
      for(auto const&d:data){
        for(int i=0;i<indent;++i)std::cout << " ";
        std::cout << d.first << ": ";
        d.second.print();
        std::cout <<  std::endl;
      }
    }
};

class Keyframes{
  public:
    std::map<int,Keyframe>keyframes;

    void print()const{
      for(auto const&key:keyframes){
        std::cout << "frame " << key.first << ": " << std::endl;
        key.second.print(2);
      }
    }
    template<typename T>
    static void moveToNext(T&it,T const&end,std::string const&n){
      it++;
      while(it!=end&&!it->second.has(n))it++;
    }
    template<typename T>
    static T find(T&it,T const& end,int f,std::string const&n){
      if(it == end)return end;
      auto prev = it;
      moveToNext(it,end,n);
      while(it != end){
        if(prev->first <= f && f < it->first && prev->second.has(n))
          return prev;
        prev = it;
        moveToNext(it,end,n);
      }
      if(prev->first <= f && prev->second.has(n))
        return prev;
      return end;
    }
    
    auto getKeyframe(int f,std::string const&n){
      auto       it   = std::begin(keyframes);
      auto const end  = std::end  (keyframes);
      return find(it,end,f,n);
    }
    auto const getKeyframe(int f,std::string const&n)const{
      auto       it   = std::cbegin(keyframes);
      auto const end  = std::cend  (keyframes);
      return find(it,end,f,n);
    }


    template<typename T>
    T get(int f,std::string const&n,T v)const{
      auto const it = getKeyframe(f,n);
      if(it == std::end(keyframes))return v;
      return it->second.get<T>(n,v);
    }

    template<typename T>
    T&get(int f,std::string const&n,T&v){
      auto it = getKeyframe(f,n);
      if(it == std::end(keyframes))return v;
      return it->second.get<T>(n);
    }

    template<typename T>
    void set(int f,std::string const&n,T v){
      //std::cerr << "set("<<f<<","<<n<<","<<v<<")"<<std::endl;
      auto w = keyframes.emplace(std::piecewise_construct,std::forward_as_tuple(f),std::forward_as_tuple(n,v));
      if(!std::get<1>(w)){
        std::get<0>(w)->second.set(n,v);
      }
    }
    void erase(int f,std::string const&n){
      //std::cerr <<"erase("<<f<<","<<n<<")"<<std::endl;
      auto it = getKeyframe(f,n);
      if(it == std::end(keyframes))return;
      auto&key = it->second;
      key.erase(n);
      if(key.data.empty())
        keyframes.erase(f);
    }

};

}

