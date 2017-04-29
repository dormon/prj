#include"renderModel.h"
#include<glm/gtc/type_ptr.hpp>

RenderMesh::RenderMesh(std::shared_ptr<ge::db::Mesh>mesh){
  this->_matrix = mesh->matrix;
  unsigned bufferSize=sizeof(float)*3*mesh->geometry->vertices->getNofVertices();

  this->_verticesVBO = new ge::gl::BufferObject(bufferSize);
  float*ptr=(float*)this->_verticesVBO->map();
  std::memcpy(ptr,mesh->geometry->positionData,bufferSize);
  this->_verticesVBO->unmap();

  this->_normalsVBO = new ge::gl::BufferObject(bufferSize);
  ptr=(float*)this->_normalsVBO->map();
  std::memcpy(ptr,mesh->geometry->normalData,bufferSize);
  this->_normalsVBO->unmap();


  bufferSize = mesh->geometry->primitives->indices.size()*sizeof(unsigned);
  this->_elementsVBO = new ge::gl::BufferObject(bufferSize);
  unsigned*iptr=(unsigned*)this->_elementsVBO->map();
  std::memcpy(iptr,mesh->geometry->primitives->indices.data(),bufferSize);
  this->_elementsVBO->unmap();

  this->_vao = new ge::gl::VertexArrayObject();
  this->_vao->addElementBuffer(this->_elementsVBO);
  this->_vao->addAttrib(this->_verticesVBO,0,3,GL_FLOAT);
  this->_vao->addAttrib(this->_nofElements,0,3,GL_FLOAT);
}

RenderMesh::~RenderMesh(){
  delete this->_verticesVBO;
  delete this->_normalsVBO;
  delete this->_elementsVBO;
  delete this->_vao;
}

void RenderMesh::draw(ge::gl::ProgramObject*program,glm::mat4 model){
  program->setdsa("modelMatrix",1,GL_FALSE,glm::value_ptr(this->_matrix*model));
  this->_vao->bind();
  glDrawElements(GL_TRIANGLES,this->_nofElements,GL_UNSIGNED_INT,NULL);
  this->_vao->unbind();
}


RenderModel::~RenderModel(){}

void RenderModel::draw(ge::gl::ProgramObject*program,glm::mat4 model){
  for(auto x:this->_renderMeshes)
    x->draw(program,x->_matrix*model);
  for(auto x:this->_renderModels)
    x->draw(program,x->_matrix*model);
}


RenderScene::~RenderScene(){}

void RenderScene::draw(ge::gl::ProgramObject*program){
  this->_root->draw(program,glm::mat4(1.f));
}


RenderScene::RenderScene(std::shared_ptr<ge::db::Scene>scene){
  //scene->root=this->_createRenderModel(scene->root);
}

void AdvancedRenderScene::draw(){
  this->_vao->bind();
  this->_dibo->bind(GL_DRAW_INDIRECT_BUFFER);
  glMultiDrawElementsIndirect(GL_TRIANGLES,GL_UNSIGNED_INT,NULL,this->_nofMeshes,sizeof(unsigned)*5);
  this->_vao->unbind();
}


#define ___ std::cerr<<__LINE__<<std::endl

AdvancedRenderScene::AdvancedRenderScene(std::shared_ptr<ge::db::Scene>scene){
  std::vector<unsigned>offsets;
  offsets.push_back(0);
  for(auto x:scene->geometries)
    offsets.push_back(offsets[offsets.size()-1]+x->primitives->indices.size());
  unsigned nofElements=offsets[offsets.size()-1];

  this->_elementsVBO = new ge::gl::BufferObject(sizeof(unsigned)*nofElements);
  unsigned*uptr=(unsigned*)this->_elementsVBO->map();
  unsigned curOffsetIndex=0;
  for(auto x:scene->geometries){
    for(auto i:x->primitives->indices){
      *uptr=i+offsets[curOffsetIndex];
      ++uptr;
    }
    curOffsetIndex++;
  }
  this->_elementsVBO->unmap();

  std::vector<unsigned>vertexOffsets;
  vertexOffsets.push_back(0);
  for(auto x:scene->geometries)
    vertexOffsets.push_back(vertexOffsets[vertexOffsets.size()-1]+x->vertices->getNofVertices());
  unsigned nofVertices=vertexOffsets[vertexOffsets.size()-1];

  this->_verticesVBO = new ge::gl::BufferObject(sizeof(float)*3*nofVertices);
  float*ptr=(float*)this->_verticesVBO->map();
  curOffsetIndex=0;
  for(auto x:scene->geometries){
    std::memcpy(ptr+3*vertexOffsets[curOffsetIndex],x->positionData,sizeof(float)*3*x->vertices->getNofVertices());
    curOffsetIndex++;
  }
  this->_verticesVBO->unmap();
  this->_normalsVBO = new ge::gl::BufferObject(sizeof(float)*3*nofVertices);
  ptr=(float*)this->_normalsVBO->map();
  curOffsetIndex=0;
  for(auto x:scene->geometries){
    std::memcpy(ptr+3*vertexOffsets[curOffsetIndex],x->normalData,sizeof(float)*3*x->vertices->getNofVertices());
    curOffsetIndex++;
  }
  this->_normalsVBO->unmap();
  this->_vao = new ge::gl::VertexArrayObject();
  this->_vao->addElementBuffer(this->_elementsVBO);
  this->_vao->addAttrib(this->_verticesVBO,0,3,GL_FLOAT);
  this->_vao->addAttrib(this->_normalsVBO ,1,3,GL_FLOAT);

  this->_dibo = new ge::gl::BufferObject(scene->geometries.size()*sizeof(unsigned)*5);
  uptr = (unsigned*)this->_dibo->map();
  for(unsigned i=0;i<scene->geometries.size();++i){
    uptr[i*5+0]=scene->geometries[i]->primitives->indices.size();
    uptr[i*5+1]=1;
    uptr[i*5+2]=offsets[i];
    uptr[i*5+3]=0;
    uptr[i*5+4]=0;
  }
  this->_dibo->unmap();
}

AdvancedRenderScene::~AdvancedRenderScene(){

}


