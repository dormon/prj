#include"Scene.h"
#include<sstream>

using namespace ge::db;

VertexAttribDataDescriptor::VertexAttribDataDescriptor(
    unsigned      components,
    enum Type     type      ,
    enum Semantic semantic  ){
  this->components = components;
  this->type       = type      ;
  this->semantic   = semantic  ;
}

unsigned VertexAttribDataDescriptor::size(){
  const unsigned typeBytes[]={
    sizeof(char              ),
    sizeof(short             ),
    sizeof(int               ),
    sizeof(long long         ),
    sizeof(unsigned char     ),
    sizeof(unsigned short    ),
    sizeof(unsigned          ),
    sizeof(unsigned long long),
    sizeof(float             ),
    sizeof(double            ),
  };
  return typeBytes[this->type]*this->components;
}

std::string VertexAttribDataDescriptor::type2Str(){
  const std::string typeNames[]={
    "I8 ",
    "I16",
    "I32",
    "I64",
    "U8 ",
    "U16",
    "U32",
    "U64",
    "F32",
    "F64"
  };
  if(this->type<=F64)return typeNames[this->type];
  std::stringstream ss;
  ss<<(unsigned)(this->type-F64-1);
  return ss.str();
}

std::string VertexAttribDataDescriptor::semantic2Str(){
  const std::string semanticNames[]={
    "POSITION",
    "COORD   ",
    "NORMAL  ",
    "BINORMAL",
    "TANGENT ",
    "COLOR   "
  };
  if(this->semantic<=COLOR)return semanticNames[this->semantic];
  std::stringstream ss;
  ss<<(unsigned)(this->semantic-COLOR-1);
  return ss.str();
}

std::string VertexAttribDataDescriptor::toStr(){
  std::stringstream ss;
  ss<<this->type2Str()<<" "<<this->components<<" "<<this->semantic2Str();
  return ss.str();
}

VertexAttrib::VertexAttrib(
    const void*ptr                           ,
    VertexAttribDataDescriptor dataDescriptor,
    unsigned   offset                        ,
    unsigned   stride                        ){
  this->_ptr            = ptr                                       ;
  this->_dataDescriptor = dataDescriptor                            ;
  this->_stride         = stride?stride:this->_dataDescriptor.size();
  this->_offset         = offset                                    ;
}

const void*VertexAttrib::get(unsigned vertex){
  return (char*)this->_ptr+this->_offset+this->_stride*vertex;
}

const void*VertexAttrib::getPtr(){
  return this->_ptr;
}

unsigned VertexAttrib::getStride(){
  return this->_stride;
}

unsigned VertexAttrib::getOffset(){
  return this->_offset;
}

VertexAttribDataDescriptor&VertexAttrib::getDataDescriptor(){
  return this->_dataDescriptor;
}

std::string VertexAttrib::toStr(){
  std::stringstream ss;
  ss<<this->_ptr   <<" ";
  ss<<this->_stride<<" ";
  ss<<this->_offset<<" ";
  ss<<this->_dataDescriptor.toStr();
  return ss.str();
}

MeshVertices::MeshVertices(unsigned nofVertices){
  this->_nofVertices = nofVertices;
}

unsigned MeshVertices::getNofVertices(){
  return this->_nofVertices;
}

unsigned MeshVertices::getNofAttribs(){
  return this->_attribs.size();
}

unsigned MeshVertices::addVertexAttrib(VertexAttrib&attrib){
  this->_attribs.push_back(attrib);
  return this->_attribs.size()-1;
}

const void*MeshVertices::getAttrib(
    unsigned index,
    unsigned vertex){
  return this->_attribs[index].get(vertex);
}

unsigned   MeshVertices::getAttribSize(unsigned index){
  return this->_attribs[index].getDataDescriptor().size();
}

std::string MeshVertices::toStr(){
  std::stringstream ss;
  ss<<this->_nofVertices<<std::endl;
  for(unsigned i=0;i<this->_attribs.size();++i)
    ss<<this->_attribs[i].toStr()<<std::endl;
  return ss.str();
}

MeshPrimitives::MeshPrimitives(
    unsigned  nofVertices            ,
    enum Type type                   ,
    unsigned  nofVerticesPerPrimitive){
  this->type                    = type                   ;
  this->nofVerticesPerPrimitive = nofVerticesPerPrimitive;
  this->indexed                 = false                  ;
  this->restartIndex            = 0xffffffff             ;
  this->enableRestart           = false                  ;
  this->nofVertices             = nofVertices            ;

}

MeshPrimitives::MeshPrimitives(
    unsigned              nofVertices            ,
    enum Type             type                   ,
    unsigned              nofVerticesPerPrimitive,
    std::vector<unsigned>&indices                ,
    bool                  enableRestart          ,
    unsigned              restartIndex           ){
  this->type                    = type                   ;
  this->nofVerticesPerPrimitive = nofVerticesPerPrimitive;
  this->indexed                 = true                   ;
  this->restartIndex            = restartIndex           ;
  this->enableRestart           = enableRestart          ;
  this->nofVertices             = nofVertices            ;
}

std::string MeshPrimitives::toStr(){
  std::stringstream ss;
  ss<<"type: "<<this->type<<std::endl;
  ss<<"nofVerticesPerPrimitive: "<<this->nofVerticesPerPrimitive<<std::endl;
  ss<<"indexed: "<<this->indexed<<std::endl;
  ss<<"nofIndices: "<<this->indices.size()<<std::endl;
  ss<<"restartIndex: "<<this->restartIndex<<std::endl;
  ss<<"enableRestart: "<<this->enableRestart<<std::endl;
  ss<<"nofVertices: "<<this->nofVertices<<std::endl;
  return ss.str();
}

void MeshGeometry::createxAttribData(
    MeshVertices*vertices,
    void**data,
    unsigned nofVertices,
    VertexAttribDataDescriptor dataDescriptor){
  *data = new char*[dataDescriptor.size()*nofVertices];
  VertexAttrib attrib((const void*)*data,dataDescriptor);
  vertices->addVertexAttrib(attrib);
}

MeshGeometry::MeshGeometry(){
  this->primitives = NULL;
  this->vertices   = NULL;
  this->positionData = NULL;
  this->normalData   = NULL;
}
MeshGeometry::~MeshGeometry(){
  if(this->primitives)delete this->primitives;this->primitives = NULL;
  if(this->vertices  )delete this->vertices  ;this->vertices   = NULL;
  if(positionData)delete[](char*)this->positionData;this->positionData = NULL;
  if(normalData  )delete[](char*)this->normalData  ;this->normalData   = NULL;
}

std::string MeshGeometry::toStr(){
  std::stringstream ss;
  ss<<this->vertices->toStr();
  ss<<this->primitives->toStr();
  return ss.str();
}

Model::~Model(){}

Scene::~Scene(){}

bool ModelLoaderManager::_canLoad(std::string regexp,std::string file){
  if(regexp=="")return true;
  //TODO
  return false;
}
void ModelLoaderManager::registerLoader(ModelLoader*loader,std::string regexp){
  this->_loaders.push_back(loader);
  this->_loadersRegExps.push_back(regexp);
}

std::shared_ptr<Scene>ModelLoaderManager::load(std::string file){
  unsigned curLoader=0;
  std::shared_ptr<Scene>scene=nullptr;
  while(curLoader<this->_loaders.size()){
    if(this->_canLoad(this->_loadersRegExps[curLoader],file))
      scene = this->_loaders[curLoader]->load(file);
    if(scene)return scene;
  }
  return scene;
}

ModelLoader::~ModelLoader(){}
