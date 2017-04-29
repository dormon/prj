#include"copyArgumentManager2Namespace.h"
#include<geCore/TypeRegister.h>

void ArgData2TypeDescriptor(std::vector<unsigned>&descriptor,ge::util::ArgData*data){
  ge::util::ArgData::Type type=data->getType();
  switch(type){
    case ge::util::ArgData::FLOAT:
      descriptor.push_back(ge::core::TypeRegister::F32);
      return;
    case ge::util::ArgData::INT:
      descriptor.push_back(ge::core::TypeRegister::I32);
      return;
    case ge::util::ArgData::STRING:
      descriptor.push_back(ge::core::TypeRegister::STRING);
      return;
    case ge::util::ArgData::ARRAY:
      descriptor.push_back(ge::core::TypeRegister::ARRAY);
      descriptor.push_back(((ge::util::ArrayData*)data)->size());
      ArgData2TypeDescriptor(descriptor,(*((ge::util::ArrayData*)data))[0]);
      return;
    case ge::util::ArgData::STRUCT:
      descriptor.push_back(ge::core::TypeRegister::STRUCT);
      descriptor.push_back(((ge::util::StructData*)data)->size());
      for(unsigned i=0;i<((ge::util::StructData*)data)->size();++i)
        ArgData2TypeDescriptor(descriptor,(*((ge::util::StructData*)data))[i]);
      return;
  }
}

void ArgData2Accessor(ge::core::Accessor ac,ge::util::ArgData*data){
  ge::util::ArgData::Type type=data->getType();
  switch(type){
    case ge::util::ArgData::FLOAT:
      ac=((ge::util::BaseData<float>*)data)->get();
      return;
    case ge::util::ArgData::INT:
      ac=((ge::util::BaseData<int>*)data)->get();
      return;
    case ge::util::ArgData::STRING:
      ac=((ge::util::BaseData<std::string>*)data)->get();
      return;
    case ge::util::ArgData::ARRAY:
      for(unsigned i=0;i<((ge::util::ArrayData*)data)->size();++i)
        ArgData2Accessor(ac[i],(*((ge::util::ArrayData*)data))[i]);
      return;
    case ge::util::ArgData::STRUCT:
      for(unsigned i=0;i<((ge::util::StructData*)data)->size();++i)
        ArgData2Accessor(ac[i],(*((ge::util::StructData*)data))[i]);
      return;
  }
}

void copyArgumentManager2Namespace(ge::util::sim::Namespace*ns,ge::util::ArgumentManager*argm,ge::core::TypeRegister*typeRegister){
  for(auto x:*argm){
    std::vector<unsigned>typeDescriptor;
    ArgData2TypeDescriptor(typeDescriptor,x.second);
    std::shared_ptr<ge::core::Accessor>sac=std::shared_ptr<ge::core::Accessor>(new ge::core::Accessor(typeRegister->allocAccessor(typeRegister->addType(/*ss.str().c_str()*/"",typeDescriptor))),[](ge::core::Accessor*ac){ac->free();delete ac;});
    ArgData2Accessor(*sac,x.second);
    ns->insert(x.first,sac);
  }
}

