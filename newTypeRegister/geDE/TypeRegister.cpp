#include<geDE/TypeRegister.h>
//#include<geDE/AtomicResource.h>
#include<sstream>
#include<memory>
#include<cassert>
#include<cstring>

using namespace ge::de;

class Function;

class TypeRegister::TypeDescription{
  public:
    TypeType type;
    TypeDescription(TypeType const&type){
      this->type = type;
    }
    virtual ~TypeDescription(){}
    virtual bool init(
        TypeRegister           *tr         ,
        DescriptionVector const&description,
        size_t                 &i          ,
        bool                    exists     ) = 0;
    virtual std::string toStr(TypeRegister const*,TypeId)const = 0;
    virtual bool equal(TypeDescription const*other)const = 0;
    TypeId findInRegister(
        TypeRegister           *tr         ,
        DescriptionVector const&description,
        size_t                 &i          ){
      assert(this!=nullptr);
      size_t old = i;
      if(!this->init(tr,description,i,true))return UNREGISTERED;
      TypeId index = 0;
      for(auto const&x:tr->_types){
        if(x->equal(this))
          return tr->_vectorIndex2TypeId(index);
        index++;
      }
      i = old;
      return UNREGISTERED;
    }
    virtual void callConstructor(TypeRegister*tr,uint8_t*ptr)const = 0;
    virtual void callDestructor(TypeRegister*tr,uint8_t*ptr)const = 0;
    virtual size_t byteSize(TypeRegister const*tr)const = 0;

    static TypeId checkAndBindType(
        TypeRegister     *tr,
        std::string const&name,
        DescriptionVector const&description,
        TypeDescription*d,
        size_t &i){
      size_t old = i;
      if(!d->init(tr,description,i,false)){
        delete d;
        return UNREGISTERED;
      }
      auto id = tr->_vectorIndex2TypeId(tr->_types.size());
      auto ii = tr->_name2TypeId.find(name);
      if(ii != tr->_name2TypeId.end()){
        std::cerr<<"ERROR: TypeDescription::checkAndBindType() - ";
        std::cerr<<"name "<<name<<" is already used with different type: "<<ii->second<<" aka "<<ii->first;
        std::cerr<<std::endl;
        i=old;
        delete d;
        return UNREGISTERED;
      }
      tr->_types.push_back(d);
      tr->_bindTypeIdWithName(id,name);
      return id;
    }

};

class TypeRegister::ArrayDescription: public TypeDescription{
  public:
    size_t size;
    TypeId elementType;
    ArrayDescription(
        TypeId const&elementType,
        size_t const&size       ):TypeDescription(ARRAY){
      this->elementType = elementType;
      this->size = size;
    }
    ArrayDescription():TypeDescription(ARRAY){}
    virtual bool init(
        TypeRegister*           tr         ,
        DescriptionVector const&description,
        size_t                 &i          ,
        bool                    exists     )override{
      assert(this!=nullptr);
      size_t old = i;
      if(i>=description.size()){i=old;return false;}
      if(description[i++]!=ARRAY){i=old;return false;}
      if(i>=description.size()){i=old;return false;}
      this->size = description[i++];
      if(exists)
        this->elementType = tr->_typeExists(description,i);
      else
        this->elementType = tr->_addType("",description,i);
      if(elementType == UNREGISTERED){i=old;return UNREGISTERED;}
      return true;
    }
    virtual ~ArrayDescription(){}
    bool operator==(ArrayDescription const&other)const{
      assert(this!=nullptr);
      return this->size == other.size && this->elementType == other.elementType;
    }
    virtual bool equal(TypeDescription const*other)const override{
      assert(this!=nullptr);
      if(this->type != other->type)return false;
      return *this==*(ArrayDescription*)other;
    }
    virtual std::string toStr(TypeRegister const*tr,TypeId)const override{
      assert(this!=nullptr);
      std::stringstream ss;
      ss<<"["<<this->size<<","<<tr->type2Str(tr->_typeId2VectorIndex(this->elementType))<<"]";
      return ss.str();
    }
    virtual void callConstructor(TypeRegister*tr,uint8_t*ptr)const override{
      assert(this!=nullptr);
      size_t elementSize = tr->computeTypeIdSize(this->elementType);
      for(size_t i=0;i<this->size;++i)
        tr->_callConstructors(ptr+elementSize*i,elementSize);
    }
    virtual void callDestructor(TypeRegister*tr,uint8_t*ptr)const override{
      assert(this!=nullptr);
      size_t elementSize = tr->computeTypeIdSize(this->elementType);
      for(size_t i=0;i<this->size;++i)
        tr->_callDestructors(ptr+elementSize*i,elementSize);
    }
    virtual size_t byteSize(TypeRegister const*tr)const override{
      assert(this!=nullptr);
      return this->size*tr->computeTypeIdSize(this->elementType);
    }
};

class TypeRegister::StructDescription: public TypeDescription{
  public:
    std::vector<TypeId>elementTypes;
    StructDescription(
        std::vector<TypeId>const&elementTypes):TypeDescription(STRUCT){
      this->elementTypes = elementTypes;
    }
    StructDescription():TypeDescription(STRUCT){}
    virtual bool init(
        TypeRegister*           tr         ,
        DescriptionVector const&description,
        size_t                 &i          ,
        bool                    exists     )override{
      assert(this!=nullptr);
      size_t old = i;
      if(i>=description.size()){i=old;return false;}
      if(description[i++]!=STRUCT){i=old;return false;}
      if(i>=description.size()){i=old;return false;}
      size_t size = description[i++];
      for(size_t e=0;e<size;++e){
        if(exists)
          this->elementTypes.push_back(tr->_typeExists(description,i));
        else
          this->elementTypes.push_back(tr->_addType("",description,i));
        if(this->elementTypes.back() == UNREGISTERED){i=old;return false;}
      }
      return true;
    }
    virtual ~StructDescription(){}
    bool operator==(StructDescription const&other)const{
      assert(this!=nullptr);
      return this->elementTypes == other.elementTypes;
    }
    virtual bool equal(TypeDescription const*other)const override{
      assert(this!=nullptr);
      if(this->type != other->type)return false;
      return *this==*(StructDescription*)other;
    }
    virtual std::string toStr(TypeRegister const*tr,TypeId)const override{
      assert(this!=nullptr);
      std::stringstream ss;
      ss<<"struct{";
      bool first=true;
      for(auto const&x:this->elementTypes){
        if(first)first = false;
        else ss<<",";
        ss<<tr->type2Str(tr->_typeId2VectorIndex(x));
      }
      ss<<"}";
      return ss.str();
    }
    virtual void callConstructor(TypeRegister*tr,uint8_t*ptr)const override{
      assert(this!=nullptr);
      size_t offset = 0;
      for(auto const&x:this->elementTypes){
        tr->_callConstructors(ptr+offset,x);
        offset += tr->computeTypeIdSize(x);
      }
    }
    virtual void callDestructor(TypeRegister*tr,uint8_t*ptr)const override{
      assert(this!=nullptr);
      size_t offset = 0;
      for(auto const&x:this->elementTypes){
        tr->_callDestructors(ptr+offset,x);
        offset += tr->computeTypeIdSize(x);
      }
    }
    virtual size_t byteSize(TypeRegister const*tr)const override{
      assert(this!=nullptr);
      size_t size = 0;
      for(auto const&x:this->elementTypes)
        size+=tr->computeTypeIdSize(x);
      return size;
    }
};

class TypeRegister::FunctionDescription: public TypeDescription{
  public:
    std::vector<TypeId>argumentTypes;
    TypeId returnType;
    FunctionDescription(
        TypeId             const&returnType   ,
        std::vector<TypeId>const&argumentTypes):TypeDescription(FCE){
      this->returnType = returnType;
      this->argumentTypes = argumentTypes;
    }
    FunctionDescription():TypeDescription(FCE){}
    virtual bool init(
        TypeRegister*           tr         ,
        DescriptionVector const&description,
        size_t                 &i          ,
        bool                    exists     )override{
      assert(this!=nullptr);
      size_t old = i;
      if(i>=description.size()){i=old;return false;}
      if(description[i++]!=FCE){i=old;return false;}
      if(exists)
        this->returnType = tr->_typeExists(description,i);
      else
        this->returnType = tr->_addType("",description,i);
      if(returnType == UNREGISTERED){i=old;return false;}
      if(i>=description.size()){i=old;return false;}
      size_t size = description[i++];
      for(size_t e=0;e<size;++e){
        if(exists)
          this->argumentTypes.push_back(tr->_typeExists(description,i));
        else
          this->argumentTypes.push_back(tr->_addType("",description,i));
        if(this->argumentTypes.back() == UNREGISTERED){i=old;return false;}
      }
      return true;
    }
    virtual ~FunctionDescription(){}
    bool operator==(FunctionDescription const&other)const{
      assert(this!=nullptr);
      return 
        this->returnType == other.returnType && 
        this->argumentTypes == other.argumentTypes;
    }
    virtual bool equal(TypeDescription const*other)const override{
      assert(this!=nullptr);
      if(this->type != other->type)return false;
      return *this==*(FunctionDescription*)other;
    }
    virtual std::string toStr(TypeRegister const*tr,TypeId)const override{
      assert(this!=nullptr);
      std::stringstream ss;
      ss<<"(";
      bool first=true;
      for(auto const&x:this->argumentTypes){
        if(first)first = false;
        else ss<<",";
        ss<<tr->type2Str(tr->_typeId2VectorIndex(x));
      }
      ss<<")->"<<tr->type2Str(tr->_typeId2VectorIndex(this->returnType));
      return ss.str();
    }
    virtual void callConstructor(TypeRegister*,uint8_t*ptr)const override{
      new(ptr)std::shared_ptr<Function>();
    }
    virtual void callDestructor(TypeRegister*,uint8_t*ptr)const override{
      ((std::shared_ptr<Function>*)ptr)->~shared_ptr();
    }
    virtual size_t byteSize(TypeRegister const*)const override{
      return sizeof(std::shared_ptr<FunctionDescription>);
    }

};

class TypeRegister::AtomicDescription: public TypeDescription{
  public:
    size_t size;
    Destructor destructor;
    Constructor constructor;
    AtomicDescription(
        size_t      const&size                 ,
        Destructor  const&destructor  = nullptr,
        Constructor const&constructor = nullptr):TypeDescription(ATOMIC){
      this->size = size;
      this->destructor  = destructor;
      this->constructor = constructor;
    }
    virtual ~AtomicDescription(){}
    virtual bool init(
        TypeRegister*           ,
        DescriptionVector const&,
        size_t                 &,
        bool                    )override{return true;}
    bool operator==(AtomicDescription const&other)const{
      assert(this!=nullptr);
      return this->size == other.size &&
        this->destructor == other.destructor &&
        this->constructor == other.constructor;
    }
    virtual bool equal(TypeDescription const*other)const override{
      assert(this!=nullptr);
      if(this->type != other->type)return false;
      return *this==*(AtomicDescription*)other;
    }
    virtual std::string toStr(TypeRegister const*tr,TypeId id)const override{
      assert(this!=nullptr);
      assert(tr->_typeId2Synonyms.count(id)!=0);
      std::stringstream ss;
      ss<<*tr->_typeId2Synonyms.find(id)->second.begin();
      return ss.str();
    }
    virtual void callConstructor(TypeRegister*,uint8_t*ptr)const override{
      assert(this!=nullptr);
      if(this->constructor)this->constructor((int8_t*)ptr);
    }
    virtual void callDestructor(TypeRegister*,uint8_t*ptr)const override{
      assert(this!=nullptr);
      if(this->destructor)this->destructor((uint8_t*)ptr);
    }
    virtual size_t byteSize(TypeRegister const*)const override{
      assert(this!=nullptr);
      return this->size;
    }

};

TypeRegister::TypeRegister(){
}

TypeRegister::~TypeRegister(){
  assert(this!=nullptr);
  for(auto const&x:this->_types)
    delete x;
}


TypeRegister::TypeId TypeRegister::addAtomicType(
    std::string const&name       ,
    size_t      const&size       ,
    Destructor  const&destructor ,
    Constructor const&constructor){
  assert(this!=nullptr);
  auto ii = this->_name2TypeId.find(name);
  if(ii!=this->_name2TypeId.end()){
    TypeId id = ii->second;
    auto index = this->_typeId2VectorIndex(id);
    assert(index<this->_types.size());
    auto type = (AtomicDescription*)this->_types[index];
    if(type->size != size)return UNREGISTERED;
    if(
        type->destructor != nullptr &&
        destructor       != nullptr && 
        type->destructor != destructor)return UNREGISTERED;
    if(
        type->constructor != nullptr &&
        constructor       != nullptr &&
        type->constructor != constructor)return UNREGISTERED;
    if(destructor)type->destructor = destructor;
    if(constructor)type->constructor = constructor;
    return id;
  }
  TypeId index = 0;
  for(auto const&x:this->_types){
    if(x->type != ATOMIC){
      index++;
      continue;
    }
    auto type = (AtomicDescription*)x;
    if(
        type->size == size &&
        type->destructor != nullptr &&
        type->constructor != nullptr &&
        type->destructor == destructor &&
        type->constructor == constructor){
      TypeId id = this->_vectorIndex2TypeId(index);
      assert(id<this->_typeId2Synonyms.size());
      if(name!="")this->_typeId2Synonyms[id].insert(name);
      return id;
    }
    index++;
  }
  TypeId id = this->_vectorIndex2TypeId(this->_types.size());
  this->_types.push_back(new AtomicDescription(size,destructor,constructor));
  this->_bindTypeIdWithName(id,name);
  return id;
}

void TypeRegister::_bindTypeIdWithName(TypeId id,std::string const&name){
  assert(this!=nullptr);
  if(name==""){
    std::stringstream ss;
    ss<<"type"<<id;
    this->_typeId2Synonyms[id].insert(ss.str());
    this->_name2TypeId[ss.str()]=id;
    return;
  }
  this->_typeId2Synonyms[id].insert(name);
  this->_name2TypeId[name]=id;
}

TypeRegister::TypeId TypeRegister::_typeExists(
    DescriptionVector const&description,
    size_t                 &i){
  assert(this!=nullptr);
  if(i>=description.size())return UNREGISTERED;
  switch(description[i]){
    case UNREGISTERED:return UNREGISTERED;
    case ATOMIC      :return UNREGISTERED;
    case ARRAY       :{ArrayDescription    desc;return desc.findInRegister(this,description,i);}
    case STRUCT      :{StructDescription   desc;return desc.findInRegister(this,description,i);}
    case FCE         :{FunctionDescription desc;return desc.findInRegister(this,description,i);}
    default          :return this->_typeIdExists(description,i);
  }
}

TypeRegister::TypeId TypeRegister::_typeIdExists(
    DescriptionVector const&description,
    size_t                 &i){
  assert(this!=nullptr);
  if(i>=description.size())return UNREGISTERED;
  if(description[i]<TYPEID)return UNREGISTERED;
  if(this->_typeId2VectorIndex(description[i])>=this->_types.size())
    return UNREGISTERED;
  return description[i++];
}

TypeRegister::TypeId TypeRegister::addCompositeType(
    std::string        const&name       ,
    DescriptionVector  const&description){
  assert(this!=nullptr);
  size_t i=0;
  return this->_addType(name,description,i);
}

TypeRegister::TypeId TypeRegister::_addType(
    std::string       const&name       ,
    DescriptionVector const&description,
    size_t                 &i          ){
  assert(this!=nullptr);
  size_t old = i;
  auto id = this->_typeExists(description,i);
  if(id != UNREGISTERED){
    auto ii = this->_name2TypeId.find(name);
    if(ii != this->_name2TypeId.end()){
      if(ii->second != id){
        std::cerr<<"ERROR: TypeRegister::_addType() - ";
        std::cerr<<"name "<<name<<" is already used with different type: "<<ii->second;
        std::cerr<<std::endl;
        i = old;
        return UNREGISTERED;
      }
      return id;
    }
    this->_bindTypeIdWithName(id,name);
    return id;
  }
  if(i>=description.size())return UNREGISTERED;
  switch(description[i]){
    case UNREGISTERED:return UNREGISTERED;
    case ATOMIC      :return UNREGISTERED;
    case ARRAY       :return TypeDescription::checkAndBindType(this,name,description,new ArrayDescription   (),i);
    case STRUCT      :return TypeDescription::checkAndBindType(this,name,description,new StructDescription  (),i);
    case FCE         :return TypeDescription::checkAndBindType(this,name,description,new FunctionDescription(),i);
    default          :return this->_addTypeId(name,description,i);
  }
}

TypeRegister::TypeId TypeRegister::_addTypeId(
    std::string       const&name       ,
    DescriptionVector const&description,
    size_t                 &i          ){
  assert(this!=nullptr);
  if(i>=description.size())return UNREGISTERED;
  if(description[i]<TYPEID)return UNREGISTERED;
  if(this->_typeId2VectorIndex(description[i])>this->_types.size())return UNREGISTERED;
  if(name!=""){
    auto ii = this->_name2TypeId.find(name);
    if(ii != this->_name2TypeId.end()){
      if(ii->second != description[i]){
        std::cerr<<"ERROR: TypeRegister::_addTypeId() - ";
        std::cerr<<"name "<<name<<" is already used with different type: "<<ii->second<<" aka "<<ii->first;
        std::cerr<<std::endl;
        return UNREGISTERED;
      }
      return description[i++];
    }
    this->_bindTypeIdWithName(description[i],name);
  }
  return description[i++];
}


TypeRegister::TypeId TypeRegister::_vectorIndex2TypeId(TypeId const&index)const{
  return index+TYPEID;
}

TypeRegister::TypeId TypeRegister::_typeId2VectorIndex(TypeId const&id)const{
  return id-TYPEID;
}


size_t TypeRegister::getNofTypes()const{
  assert(this!=nullptr);
  return this->_types.size();
}

std::string TypeRegister::type2Str(size_t index)const{
  assert(this!=nullptr);
  assert(index<this->_types.size());
  return this->_types.at(index)->toStr(this,this->_vectorIndex2TypeId(index));
}

TypeRegister::TypeType TypeRegister::getTypeIdType(TypeId id)const{
  assert(this!=nullptr);
  return this->_getDescription(id)->type;
}

size_t TypeRegister::getNofStructElements(TypeId id)const{
  assert(this!=nullptr);
  assert((StructDescription*)this->_getDescription(id)!=nullptr);
  return ((StructDescription*)this->_getDescription(id))->elementTypes.size();
}

TypeRegister::TypeId TypeRegister::getStructElementTypeId(TypeId id,size_t index)const{
  assert(this!=nullptr);
  assert((StructDescription*)this->_getDescription(id)!=nullptr);
  assert(index<((StructDescription*)this->_getDescription(id))->elementTypes.size());
  return ((StructDescription*)this->_getDescription(id))->elementTypes[index];
}

size_t TypeRegister::getArraySize(TypeId id)const{
  assert(this!=nullptr);
  assert((ArrayDescription*)this->_getDescription(id)!=nullptr);
  return ((ArrayDescription*)this->_getDescription(id))->size;
}

TypeRegister::TypeId TypeRegister::getArrayElementTypeId(TypeId id)const{
  assert(this!=nullptr);
  assert((ArrayDescription*)this->_getDescription(id)!=nullptr);
  return ((ArrayDescription*)this->_getDescription(id))->elementType;
}

TypeRegister::TypeId TypeRegister::getFceReturnTypeId(TypeId id)const{
  assert(this!=nullptr);
  assert((FunctionDescription*)this->_getDescription(id)!=nullptr);
  return ((FunctionDescription*)this->_getDescription(id))->returnType;
}

size_t TypeRegister::getNofFceArgs(TypeId id)const{
  assert(this!=nullptr);
  assert((FunctionDescription*)this->_getDescription(id)!=nullptr);
  return ((FunctionDescription*)this->_getDescription(id))->argumentTypes.size();
}

TypeRegister::TypeId TypeRegister::getFceArgTypeId(TypeId id,size_t index)const{
  assert(this!=nullptr);
  assert((FunctionDescription*)this->_getDescription(id)!=nullptr);
  assert(index<((FunctionDescription*)this->_getDescription(id))->argumentTypes.size());
  return ((FunctionDescription*)this->_getDescription(id))->argumentTypes[index];
}

TypeRegister::TypeId TypeRegister::getTypeId(std::string const&name)const{
  assert(this!=nullptr);
  assert(this->_name2TypeId.count(name)!=0);
  return this->_name2TypeId.find(name)->second;
}

std::string const& TypeRegister::getTypeIdName(TypeId id)const{
  assert(this!=nullptr);
  assert(this->_typeId2Synonyms.count(id)!=0);
  return *this->_typeId2Synonyms.find(id)->second.begin();
}

std::set<std::string>const& TypeRegister::getTypeIdSynonyms(TypeId id)const{
  assert(this!=nullptr);
  assert(this->_typeId2Synonyms.count(id)!=0);
  return this->_typeId2Synonyms.find(id)->second;
}

bool TypeRegister::hasSynonyms(TypeId id)const{
  assert(this!=nullptr);
  assert(this->_typeId2Synonyms.count(id)!=0);
  return this->_typeId2Synonyms.find(id)->second.size()>1;
}

bool TypeRegister::areSynonyms(std::string const&name0,std::string const&name1)const{
  assert(this!=nullptr);
  if(this->_name2TypeId.count(name0)==0)return false;
  if(this->_name2TypeId.count(name1)==0)return false;
  return this->getTypeId(name0)==this->getTypeId(name1);
}


size_t TypeRegister::computeTypeIdSize(TypeId id)const{
  assert(this!=nullptr);
  switch(this->getTypeIdType(id)){
    case ARRAY :
    case STRUCT:
    case FCE   :
    case ATOMIC:return this->_getDescription(id)->byteSize(this);
    default    :return 0;
  }
}

void*TypeRegister::alloc(TypeId id)const{
  assert(this!=nullptr);
  size_t size = this->computeTypeIdSize(id);
  uint8_t*ptr=new uint8_t[size];
  std::memset(ptr,0,size);
  return ptr;
}

void TypeRegister::free(void*ptr)const{
  assert(this!=nullptr);
  delete[](uint8_t*)ptr;
}

void*TypeRegister::construct(TypeId id)const{
  assert(this!=nullptr);
  auto ptr=this->alloc(id);
  this->_callConstructors((uint8_t*)ptr,id);
  return ptr;
}

void TypeRegister::destroy(void*ptr,TypeId id)const{
  this->_callDestructors((uint8_t*)ptr,id);
  this->free(ptr);
}

void TypeRegister::_callConstructors(uint8_t*ptr,TypeId id)const{
  assert(this!=nullptr);
  switch(this->getTypeIdType(id)){
    case ATOMIC:
    case ARRAY :
    case STRUCT:
    case FCE   :this->_getDescription(id)->callConstructor((TypeRegister*)this,ptr);break;
    default    :break;
  }
}

void TypeRegister::_callDestructors(uint8_t*ptr,TypeId id)const{
  assert(this!=nullptr);
  switch(this->getTypeIdType(id)){
    case ATOMIC:
    case ARRAY :
    case STRUCT:
    case FCE   :this->_getDescription(id)->callDestructor((TypeRegister*)this,ptr);break;
    default    :break;
  }
}

TypeRegister::TypeDescription*TypeRegister::_getDescription(TypeId id)const{
  assert(this!=nullptr);
  assert(this->_typeId2VectorIndex(id)<this->_types.size());
  return this->_types.at(this->_typeId2VectorIndex(id));
}

void TypeRegister::addDestructor(TypeId id,Destructor const&destructor){
  assert(this!=nullptr);
  assert(this->_getDescription(id)->type == ATOMIC);
  ((AtomicDescription*)this->_getDescription(id))->destructor = destructor;
}

void TypeRegister::addConstructor(TypeId id,Constructor const&constructor){
  assert(this!=nullptr);
  assert(this->_getDescription(id)->type == ATOMIC);
  ((AtomicDescription*)this->_getDescription(id))->constructor = constructor;
}

std::shared_ptr<Resource>TypeRegister::sharedResource(TypeId id)const{
  assert(this!=nullptr);
  (void)id;
  return nullptr;
  //return std::make_shared<AtomicResource>(std::const_pointer_cast<TypeRegister>(this->shared_from_this()),this->alloc(id),id);
}

std::shared_ptr<Resource>TypeRegister::sharedResource(std::string const&name)const{
  assert(this!=nullptr);
  return this->sharedResource(this->getTypeId(name));
}

std::shared_ptr<Resource>TypeRegister::sharedEmptyResource(TypeId id)const{
  assert(this!=nullptr);
  (void)id;
  return nullptr;
}

std::shared_ptr<Resource>TypeRegister::sharedEmptyResource(std::string const&name)const{
  assert(this!=nullptr);
  return this->sharedEmptyResource(this->getTypeId(name));
}



