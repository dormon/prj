#include<iostream>
#include<sstream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

#include<geGL/geGL.h>
#include<geCore/TypeRegister.h>
using namespace ge::core;

namespace ge{
  namespace core{
    template<>inline std::string TypeRegister::getTypeKeyword<ge::gl::ProgramObject>(){return "ge::gl::ProgramObject";}
  }
}

template<typename...ARGS>
void constructList(std::shared_ptr<TypeRegister>const&,std::vector<TypeRegister::DescriptionElement>&);

template<TypeRegister::DescriptionElement,typename...>
void constructList_helper(std::shared_ptr<TypeRegister>const&,std::vector<TypeRegister::DescriptionElement>&);
template<typename,typename...>
void constructList_helper(std::shared_ptr<TypeRegister>const&,std::vector<TypeRegister::DescriptionElement>&);

template<TypeRegister::DescriptionElement elem,typename...ARGS>
void constructList_helper(std::shared_ptr<TypeRegister>const&tr,std::vector<TypeRegister::DescriptionElement>&list){
  list.push_back(elem);
  //constructList<ARGS...>(tr,list);
}

template<typename TYPE,typename...ARGS>
void constructList_helper(std::shared_ptr<TypeRegister>const&tr,std::vector<TypeRegister::DescriptionElement>&list){
  list.push_back(tr->addClassCD<TYPE>(TypeRegister::getTypeKeyword<TYPE>()));
  constructList<ARGS...>(tr,list);
}

template<typename...ARGS>
void constructList(std::shared_ptr<TypeRegister>const&tr,std::vector<TypeRegister::DescriptionElement>&list){
  constructList_helper<ARGS...>(tr,list);
}

template<>
void constructList(std::shared_ptr<TypeRegister>const&tr,std::vector<TypeRegister::DescriptionElement>&list){
}



template<typename...ARGS>
TypeRegister::TypeID addType(std::shared_ptr<TypeRegister>const&tr,std::string name){
  std::vector<TypeRegister::DescriptionElement>list;
  constructList<ARGS...>(tr,list);
  return tr->addType(name,list);
}

int main(){
  auto tr=std::make_shared<ge::core::TypeRegister>();
  std::vector<TypeRegister::DescriptionElement>list;
//  constructList<(TypeRegister::DescriptionElement)TypeRegister::FCE,uint32_t,(TypeRegister::DescriptionElement)1,uint32_t>(tr,list);
//  constructList(tr,list);
  constructList<uint32_t>(tr,list);
  constructList<uint32_t,uint32_t>(tr,list);
  constructList<(TypeRegister::DescriptionElement)1>(tr,list);
  return 0;
}
