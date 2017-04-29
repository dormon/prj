#include"LsDirInterface.h"

LsDir::LsDir(std::string dir){
  this->_currentDir = dir;
}
LsDir::~LsDir(){
}
std::string LsDir::getFile(unsigned i){
  if(!this||i>=this->getNofFiles())return"";
  return this->_dirFiles[i];
}
unsigned LsDir::getNofFiles(){
  if(!this)return 0;
  return this->_dirFiles.size();
}
std::string LsDir::getDir(unsigned i){
  if(!this||i>=this->getNofDirs())return"";
  return this->_dirDirs[i];
}
unsigned LsDir::getNofDirs(){
  if(!this)return 0;
  return this->_dirDirs.size();
}
std::string LsDir::getCurrentDir(){
  if(!this)return"";
  return this->_currentDir;
}
void LsDir::cd(std::string){
}
