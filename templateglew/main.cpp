#include<iostream>

/*
void printData(const void*const*data){
  std::cerr<<"korektni verze"<<std::endl;
  std::cout<<data<<std::endl;
}
typedef void(*PFNPRINTDATA)(const void*const*);
// */
//*
void printData(const void**data){
  std::cerr<<"nekorektni verze"<<std::endl;
  std::cout<<data<<std::endl;
}
typedef void(*PFNPRINTDATA)(const void**);
// */


template<typename FCE,typename DATA>
void volej(DATA data);

//*
template<>void volej<void(*)(const void**     ),const void**     >(const void**      data){printData(                   data);}
template<>void volej<void(*)(const void*const*),const void**     >(const void**      data){printData((const void*const*)data);}
template<>void volej<void(*)(const void**     ),const void*const*>(const void*const* data){printData((const void**)     data);}
template<>void volej<void(*)(const void*const*),const void*const*>(const void*const* data){printData(                   data);}
// */

/*
template<void(*)(const void**     ),const void**     >void volej(const void**      data){printData(                   data);}
template<void(*)(const void**     ),const void*const*>void volej(const void*const* data){printData((const void**)     data);}
template<void(*)(const void*const*),const void**     >void volej(const void**      data){printData((const void*const*)data);}
template<void(*)(const void*const*),const void*const*>void volej(const void*const* data){printData(                   data);}
// */
int main(){
  const void*const*asd=NULL;
  volej<PFNPRINTDATA,const void*const*>(asd);
//  printData(asd);
  return 0;
}
