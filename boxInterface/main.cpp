#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

class ActionOutputPin;
class ActionInputPin;
class ResourceReadPin;
class ResourceWritePin;

class ResourceInterface;
class ActionInterface;

class ResourceReadPin{
  protected:
    std::set<std::shared_ptr<ActionInputPin>>_actions;
  public:
    ResourceReadPin(){}
    void bind(std::shared_ptr<ActionInterface>actionInterface,std::string name){
      this->_actions.insert((*actionInterface)[name]);
    }
};

class ResourceWritePin{
  protected:
    std::set<std::shared_ptr<ActionOutputPin>>_actions;
  public:
    ResourceWritePin(){}
    void bind(std::shared_ptr<ActionInterface>actionInterface,std::string name){
      this->_actions.insert((*actionInterface)[name]);
    }
};

class ActionOutputPin{
  protected:
    std::set<std::shared_ptr<ResourceWritePin>>_resources;
  public:
    ActionOutputPin(){}
    void bind(std::shared_ptr<ResourceInterfaceresourceInterface,std::string name){
      this->_resources.insert((*resourceInterface)[name]);
    }
};

class ActionInputPin{
  protected:
    std::set<std::shared_ptr<ResourceReadPin>>_resources;
  public:
    ActionOutputPin(){}
    void bind(ResourceInterface*resourceInterface,std::string name){
      this->_resources.insert((*resourceInterface)[name]);
    }
};

class ResourceReadInterface: public std::map<std::string,std::shared_ptr<ResourceReadPin>>{

};

class ResourceWriteInterface: public std::map<std::string,std::shared_ptr<ResourceWritePin>>{

};

class ActionOutputInterface: public std::map<std::string,std::shared_ptr<ActionOutputPin>>{

};

class ActionInputInterface: public std::map<std::string,std::shared_ptr<ActionInputPin>>{

};

class ResourceInterface: public ResourceReadInterface, public ResourceWriteInterface{

};

class ActionInterface: public ActionOutputInterface, public ActionInputInterface{

};

/*
class Rotation: public{
  public:
    

};
*/

int main(){
  return 0;
}
