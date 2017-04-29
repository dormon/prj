#include"model.h"

AtomicModelDescription::AtomicModelDescription(
    ModelId const&base):ModelDescription(ATOMIC,base){
}

VectorModelDescription::VectorModelDescription(
    ModelId const&base ,
    ModelId const&inner):ModelDescription(VECTOR,base),inner(inner){
}

CompositeModelDescription::CompositeModelDescription(
    ModelId          const&base  ,
    std::set<ModelId>const&inners):ModelDescription(COMPOSITE,base),inners(inners){
}

ModelId Domain::addAtomicModel(
    std::string const&name     ,
    ModelId     const&baseModel){
  assert(this != nullptr);
  auto ii = this->name2Id.find(name);
  if(ii != this->name2Id.end())return ii->second;

  auto id = this->models.size();
  if(baseModel >= this->models.size())
    this->models[id] = new AtomicModelDescription(baseModel);
  else
    this->models[id] = new AtomicModelDescription(id       );

  this->id2Name[id  ] = name;
  this->name2Id[name] = id  ;

  return id;
}

ModelId Domain::addVectorModel(
    std::string const&name      ,
    ModelId     const&baseModel ,
    ModelId     const&innerModel){
  assert(this != nullptr);

  if(baseModel >= this->models.size()){
    ge::core::printError(
        GE_CORE_FCENAME,
        "Base model id is not registered.",
        name,baseModel,innerModel);
    return Domain::nonexistingModel;
  }

  if(innerModel >= this->models.size()){
    ge::core::printError(
        GE_CORE_FCENAME,
        "Inner model id is not registered.",
        name,baseModel,innerModel);
    return Domain::nonexistingModel;
  }

  auto ii = this->name2Id.find(name);
  if(ii != this->name2Id.end()){
    auto const mptr = this->models.at(ii->second);

    if(mptr->type != ModelDescription::VECTOR){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Different type of model with that name already exists.",
          name,baseModel,innerModel);
      return Domain::nonexistingModel;
    }
    
    auto const vm = reinterpret_cast<VectorModelDescription const*>(mptr);

    if(vm->base != baseModel){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Vector model with that name already exists, "
          "but it has different base model.",
          name,baseModel,innerModel);
      return Domain::nonexistingModel;
    }

    if(vm->inner != innerModel){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Vector model with that name already exists, "
          "but it has different inner model.",
          name,baseModel,innerModel);
      return Domain::nonexistingModel;
    }

    return ii->second;
  }

  auto id = this->models.size();
  this->models[id] = new VectorModelDescription(baseModel,innerModel);

  this->id2Name[id  ] = name;
  this->name2Id[name] = id  ;

  return id;
}

ModelId Domain::addCompositeModel(
    std::string      const&name       ,
    ModelId          const&baseModel  ,
    std::set<ModelId>const&innerModels){
  assert(this != nullptr);

  if(baseModel >= this->models.size()){
    ge::core::printError(
        GE_CORE_FCENAME,
        "Base model id is not registered.",
        name,baseModel,innerModels);
    return Domain::nonexistingModel;
  }

  for(auto const&x:innerModels){
    if(x >= this->models.size()){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Inner model: "+ge::core::value2str(x)+" is not registered",
          name,baseModel,innerModels);
      return Domain::nonexistingModel;
    }
  }

  auto ii = this->name2Id.find(name);
  if(ii != this->name2Id.end()){
    auto const mptr = this->models.at(ii->second);

    if(mptr->type != ModelDescription::COMPOSITE){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Different type of model with that name already exists.",
          name,baseModel,innerModels);
      return Domain::nonexistingModel;
    }

    auto const cm = reinterpret_cast<CompositeModelDescription const*>(mptr);

    if(cm->base != baseModel){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Composite model with that name already exists, "
          "but it has different base model.",
          name,baseModel,innerModels);
      return Domain::nonexistingModel;
    }

    if(cm->inners != innerModels){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Composite model with that name already exists, "
          "but it has different inner models.",
          name,baseModel,innerModels);
      return Domain::nonexistingModel;
    }

    return ii->second;
  }

  auto id = this->models.size();
  this->models[id] = new CompositeModelDescription(baseModel,innerModels);

  this->id2Name[id  ] = name;
  this->name2Id[name] = id  ;

  return id;
}

ModelId const Domain::nonexistingModel = std::numeric_limits<ModelId>::max();
