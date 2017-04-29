#include<iostream>

struct sa{
};

struct sb: sa{
};

template<unsigned... Is>
struct seq { };

template<unsigned N, unsigned... Is>
struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

template<unsigned... Is>
struct gen_seq<0, Is...> : seq<Is...> { };

template<unsigned SIZE,typename TYPE>
void argsToArray(TYPE data[SIZE],TYPE v){
  data[SIZE-1]=v;
}

template<unsigned SIZE,typename TYPE,typename... Args>
inline void argsToArray(TYPE data[SIZE],TYPE v, Args... args){
  data[SIZE-1-sizeof...(args)]=v;
  argsToArray<SIZE,TYPE>(data,args...);
}



template<typename...>
inline void printr(int a){
  std::cout<<" "<<a;
}

template<typename... ARGS>
inline void printr(int a,ARGS... args){
  printr(a);
  printr(args...);
}

template<typename... ARGS>
void print(ARGS... args){
  std::cout<<sizeof...(args)<<"arg:";
  printr(args...);
  std::cout<<std::endl;
}



/*
void print(int a                  ){std::cout<<"1arg: "<<a                        <<std::endl;}
void print(int a,int b            ){std::cout<<"2arg: "<<a<<" "<<b                <<std::endl;}
void print(int a,int b,int c      ){std::cout<<"3arg: "<<a<<" "<<b<<" "<<c        <<std::endl;}
void print(int a,int b,int c,int d){std::cout<<"4arg: "<<a<<" "<<b<<" "<<c<<" "<<d<<std::endl;}
*/



template<unsigned DIM>
class Set{
  protected:
    int _data[DIM];
    template<unsigned... Is>
    inline void _iind(/*seq<Is...>*/){
      print(this->_data[Is]...);
    }
  public:
    template<typename... ARGS>
    Set(ARGS...args){
      argsToArray<DIM,int>(this->_data,args...);
    }
    void operator()();
};

//template<unsigned D>void Set<D>::operator()(){this->_iind(gen_seq<D>{});}

template<>void Set<4>::operator()(){this->_iind<0,1,2,3>();}
template<>void Set<3>::operator()(){this->_iind<0,1,2  >();}
template<>void Set<2>::operator()(){this->_iind<0,1    >();}
template<>void Set<1>::operator()(){this->_iind<0      >();}

int main(){
  Set<4>(32 ,   2,123,9)();
  Set<2>(124,  89      )();
  Set<2>(-12,3232      )();
  return 0;
}

