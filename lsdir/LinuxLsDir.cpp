#include"LsDirInterface.h"
#include<dirent.h>
#include<vector>

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

}
