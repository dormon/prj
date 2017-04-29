#include"LsDirInterface.h"
#include<dirent.h>
#include<vector>

#include<geUtil/RuntimeClassInterface.h>

extern "C"{

class LinuxLsDir: public LsDir{
  protected:
    virtual void _getDirContent(){
      DIR*           dir;
      struct dirent* ent;
      dir=opendir(this->_currentDir.c_str());
      if(!dir){
        this->_dirFiles.clear();
        this->_dirDirs .clear();
        return;
      }
      this->_dirDirs .clear();
      this->_dirFiles.clear();
      while((ent=readdir(dir))!=NULL){
        if(ent->d_type == DT_DIR)
          this->_dirDirs .push_back(std::string(ent->d_name));
        else
          this->_dirFiles.push_back(std::string(ent->d_name));
      }
      closedir(dir);
    }
    void _cdGoUp(){
      if(this->_currentDir=="/")return;
      if(this->_currentDir[this->_currentDir.size()-1]=='/')
        this->_currentDir=this->_currentDir.substr(0,this->_currentDir.size()-1);
      //std::cerr<<this->_currentDir<<std::endl;
      std::size_t pos=this->_currentDir.rfind("/");
      if(pos!=std::string::npos){
        this->_currentDir=this->_currentDir.substr(0,pos);
        if(this->_currentDir=="")this->_currentDir="/";
      }else{
        std::cerr<<"we cant go up in dir: "<<this->_currentDir<<std::endl;
        return;
      }
    }
    void _cdGoDown(std::string dir){
      if(this->_currentDir[this->_currentDir.size()-1]=='/')
        this->_currentDir+=dir;
      else
        this->_currentDir+="/"+dir;
    }
    void _cdCommand(std::string command){
      if(command=="")return;
      if(command.find("/")==0){
        this->_currentDir="/";
        this->_cdCommand(command.substr(1));
      }else if(command.find("./")==0){
        this->_cdCommand(command.substr(2));
      }else if(command.find("../")==0){
        this->_cdGoUp();
        this->_cdCommand(command.substr(3));
      }else if(command.find("..")==0){
        this->_cdGoUp();
        this->_cdCommand(command.substr(2));
      }else if(command.find(".")==0){
        this->_cdCommand(command.substr(1));
      }else{
        std::size_t pos=command.find("/");
        if(pos!=std::string::npos){
          this->_cdGoDown(command.substr(0,pos));
          this->_cdCommand(command.substr(pos+1));
        }else{
          this->_cdGoDown(command);
        }
      }
    }
  public:
    LinuxLsDir(std::string dir="/"):LsDir(dir){
      this->_getDirContent();
    }
    virtual ~LinuxLsDir(){

    }
    void cd(std::string dir){
      this->_cdCommand(dir);
      this->_getDirContent();
    }

};

LsDir*getObject(){
  return new LinuxLsDir();
}

static const ge::util::ClassMetaData classMetaData={
  "LsDir",
  {
    {{ge::core::TypeRegister::STRING},
      "getFile",
      {
        {{ge::core::TypeRegister::U32},"i"},
      }},
    {{ge::core::TypeRegister::U32},
      "getNofFiles",
      {
      }},
    {{ge::core::TypeRegister::STRING},
      "getDir",
      {
        {{ge::core::TypeRegister::U32},"i"},
      }},
    {{ge::core::TypeRegister::U32},
      "getNofDirs",
      {
      }},
    {{ge::core::TypeRegister::VOID},
      "cd",
      {
        {{ge::core::TypeRegister::STRING},"dir"},
      }},
    {{ge::core::TypeRegister::STRING},
      "getCurrentDir",
      {
      }},
  },{
    {{{{ge::core::TypeRegister::STRING},"dir"}}},
    {{}},
  }
};

class LinuxLsDirInterface: public ge::util::RuntimeClassInterface{
  public:
    LinuxLsDirInterface(std::shared_ptr<ge::core::TypeRegister>&typeRegister);
    virtual ~LinuxLsDirInterface();
    virtual void call(void*instance,std::string fce,std::vector<ge::core::Accessor>&args);
    virtual void*construct(std::vector<ge::core::Accessor>&args)const;
    virtual void destroy(void*instance)const;
};

LinuxLsDirInterface::LinuxLsDirInterface(std::shared_ptr<ge::core::TypeRegister>&typeRegister):RuntimeClassInterface(typeRegister,classMetaData){}

LinuxLsDirInterface::~LinuxLsDirInterface(){}

void LinuxLsDirInterface::call(void*instance,std::string fce,std::vector<ge::core::Accessor>&args){
  if(!instance){
    std::cerr<<"ERROR: can't call "<<fce<<" on empty instance"<<std::endl;
    return;
  }
  LsDir*ls=(LsDir*)instance;
  if(fce=="getFile"      )args[0]=ls->getFile(args[1]);
  if(fce=="getNofFiles"  )args[0]=ls->getNofFiles();
  if(fce=="getDir"       )args[0]=ls->getDir(args[1]);
  if(fce=="getNofDirs"   )args[0]=ls->getNofDirs();
  if(fce=="cd"           )ls->cd(args[0]);
  if(fce=="getCurrentDir")args[0]=ls->getCurrentDir();
}

void*LinuxLsDirInterface::construct(std::vector<ge::core::Accessor>&args)const{
  for(unsigned i=0;i<this->getNofConstructors();++i)
    if(this->_constructorArgsMatch(args,i))return new LinuxLsDir();
  std::cerr<<"ERROR: no matching constructor that takes "<<args.size()<<" arguments";
  if(args.size()){
    std::cerr<<":";
    for(unsigned i=0;i<args.size();++i)
      std::cerr<<" "<<this->_typeRegister->getTypeIdName(args[i].getId());
  }
  std::cerr<<std::endl;
  return nullptr;
}

void LinuxLsDirInterface::destroy(void*instance)const{
  delete (LsDir*)instance;
}

ge::util::RuntimeClassInterface*getInterface(std::shared_ptr<ge::core::TypeRegister>&typeRegister){
  return new LinuxLsDirInterface(typeRegister);
}

}
