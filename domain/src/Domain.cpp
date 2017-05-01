#include<Domain.h>
#include<AtomicModelDescription.h>
#include<VectorModelDescription.h>
#include<CompositeModelDescription.h>

#include<AtomicCommandDescription.h>

bool Domain::_checkBaseModels(
    std::set<ModelId>const&bases)const{
  assert(this != nullptr);
  for(auto const&x:bases)
    if(x >= this->models.size()){
      ge::core::printError(GE_CORE_FCENAME,
          "Base model: "+ge::core::value2str(x)+" is not registered.",
          bases);
      return false;
    }
  return true;
}

ModelId Domain::_addModel(
    std::string      const&name ,
    ModelDescription*const&model){
  assert(this != nullptr);

  auto id = this->models.size();

  this->models[id] = model;

  this->model2Name[id  ] = name;
  this->name2Model[name] = id  ;

  return id;
}

CommandId Domain::_addCommand(
    std::string        const&name   ,
    CommandDescription*const&command){
  assert(this != nullptr);

  auto id = this->commands.size();

  this->commands[id] = command;

  this->command2Name[id  ] = name;
  this->name2Command[name] = id  ;

  return id;
}


ModelId Domain::addAtomicModel(
    std::string      const&name      ,
    std::set<ModelId>const&baseModels){
  assert(this != nullptr);
  if(!this->_checkBaseModels(baseModels))
    return Domain::nonexistingModel;

  auto ii = this->name2Model.find(name);
  if(ii != this->name2Model.end())return ii->second;

  auto id = this->_addModel(name,new AtomicModelDescription(baseModels));
  if(baseModels.empty())
    this->models.at(id)->bases = {id};
  return id;

}

ModelId Domain::addAtomicModel(
    std::string          const&name ,
    std::set<std::string>const&bases){
  assert(this != nullptr);
  std::set<ModelId>b;
  for(auto const&x:bases)
    b.insert(this->getModelId(x));
  return this->addAtomicModel(name,b);
}

ModelId Domain::addVectorModel(
    std::string          const&name      ,
    std::set<std::string>const&bases     ,
    std::string          const&innerModel){
  assert(this != nullptr);
  std::set<ModelId>b;
  for(auto const&x:bases)
    b.insert(this->getModelId(x));
  return this->addVectorModel(name,b,this->getModelId(innerModel));
}

ModelId Domain::addCompositeModel(
    std::string          const&name       ,
    std::set<std::string>const&bases      ,
    std::set<std::string>const&innerModels){
  assert(this != nullptr);
  std::set<ModelId>b;
  for(auto const&x:bases)
    b.insert(this->getModelId(x));
  std::set<ModelId>i;
  for(auto const&x:innerModels)
    i.insert(this->getModelId(x));
  return this->addCompositeModel(name,b,i);
}



ModelId Domain::addVectorModel(
    std::string      const&name      ,
    std::set<ModelId>const&baseModels,
    ModelId          const&innerModel){
  assert(this != nullptr);
  if(!this->_checkBaseModels(baseModels))
    return Domain::nonexistingModel;

  if(innerModel >= this->models.size()){
    ge::core::printError(
        GE_CORE_FCENAME,
        "Inner model id is not registered.",
        name,baseModels,innerModel);
    return Domain::nonexistingModel;
  }

  auto ii = this->name2Model.find(name);
  if(ii != this->name2Model.end()){
    auto const mptr = this->models.at(ii->second);

    if(mptr->type != ModelDescription::Type::VECTOR){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Different type of model with that name already exists.",
          name,baseModels,innerModel);
      return Domain::nonexistingModel;
    }

    auto const vm = reinterpret_cast<VectorModelDescription const*>(mptr);

    if(vm->bases != baseModels){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Vector model with that name already exists, "
          "but it has different base model.",
          name,baseModels,innerModel);
      return Domain::nonexistingModel;
    }

    if(vm->inner != innerModel){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Vector model with that name already exists, "
          "but it has different inner model.",
          name,baseModels,innerModel);
      return Domain::nonexistingModel;
    }

    return ii->second;
  }
  return this->_addModel(name,new VectorModelDescription(baseModels,innerModel));
}

ModelId Domain::addCompositeModel(
    std::string      const&name       ,
    std::set<ModelId>const&baseModels ,
    std::set<ModelId>const&innerModels){
  assert(this != nullptr);
  if(!this->_checkBaseModels(baseModels))
    return Domain::nonexistingModel;

  for(auto const&x:innerModels){
    if(x >= this->models.size()){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Inner model: "+ge::core::value2str(x)+" is not registered",
          name,baseModels,innerModels);
      return Domain::nonexistingModel;
    }
  }

  auto ii = this->name2Model.find(name);
  if(ii != this->name2Model.end()){
    auto const mptr = this->models.at(ii->second);

    if(mptr->type != ModelDescription::Type::COMPOSITE){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Different type of model with that name already exists.",
          name,baseModels,innerModels);
      return Domain::nonexistingModel;
    }

    auto const cm = reinterpret_cast<CompositeModelDescription const*>(mptr);

    if(cm->bases != baseModels){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Composite model with that name already exists, "
          "but it has different base model.",
          name,baseModels,innerModels);
      return Domain::nonexistingModel;
    }

    if(cm->inners != innerModels){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Composite model with that name already exists, "
          "but it has different inner models.",
          name,baseModels,innerModels);
      return Domain::nonexistingModel;
    }

    return ii->second;
  }

  return this->_addModel(name,new CompositeModelDescription(baseModels,innerModels));
}

ModelId Domain::getModelId(std::string const&name)const{
  assert(this != nullptr);
  auto const it = this->name2Model.find(name);
  if(it == this->name2Model.end())
    return Domain::nonexistingModel;
  return it->second;
}

CommandId Domain::addAtomicCommand(
    std::string              const&name  ,
    std::vector<CommandInput>const&inputs){
  assert(this != nullptr);

  auto ii = this->name2Command.find(name);
  if(ii != this->name2Model.end())return ii->second;

  auto id = this->_addCommand(name,new AtomicCommandDescription(inputs));

  return id;
}

CommandId Domain::addBodyCommand(
    std::string                     const&name   ,
    std::vector<CommandId>          const&cmds   ,
    std::vector<std::vector<size_t>>const&mapping){
  assert(this != nullptr);

  for(auto const&x:cmds){
    if(x >= this->commands.size()){
      ge::core::printError(
          GE_CORE_FCENAME,
          "Inner command: "+ge::core::value2str(x)+" is not registered",
          name,cmds,mapping);
      return Domain::nonexistingModel;
    }
  }

  //TODO semantic check
  return 0;
  /*

     auto ii = this->name2Command.find(name);
     if(ii != this->name2Model.end()){
     auto const mptr = this->commands.at(ii->second);

     if(mptr->type != CommandDescription::Type::BODY){
     ge::core::printError(
     GE_CORE_FCENAME,
     "Different type of model with that name already exists.",
     name,baseModels,innerModels);
     return Domain::nonexistingModel;
     }

     auto const cm = reinterpret_cast<CompositeModelDescription const*>(mptr);

     if(cm->bases != baseModels){
     ge::core::printError(
     GE_CORE_FCENAME,
     "Composite model with that name already exists, "
     "but it has different base model.",
     name,baseModels,innerModels);
     return Domain::nonexistingModel;
     }

     if(cm->inners != innerModels){
     ge::core::printError(
     GE_CORE_FCENAME,
     "Composite model with that name already exists, "
     "but it has different inner models.",
     name,baseModels,innerModels);
     return Domain::nonexistingModel;
     }

     return ii->second;
     }

     return this->_addModel(name,new CompositeModelDescription(baseModels,innerModels));
     */
}

CommandId Domain::addAtomicCommand(
    std::string             const&name    ,
    std::vector<std::string>const&models  ,
    std::string             const&accesses){
  assert(this != nullptr);
  assert(models.size() == accesses.length());
  std::vector<CommandInput>inputs;
  for(size_t i=0;i<models.size();++i)
    if     (accesses.at(i)=='w')
      inputs.emplace_back(this->getModelId(models.at(i)),CommandInput::AccessType::COMMAND_WRITES);
    else if(accesses.at(i)=='r')
      inputs.emplace_back(this->getModelId(models.at(i)),CommandInput::AccessType::COMMAND_READS );
    else 
      inputs.emplace_back(this->getModelId(models.at(i)),CommandInput::AccessType::COMMAND_READS_AND_WRITES );
  return this->addAtomicCommand(name,inputs);
}




ModelId   const Domain::nonexistingModel   = std::numeric_limits<ModelId  >::max();
CommandId const Domain::nonexistingCommand = std::numeric_limits<CommandId>::max();
