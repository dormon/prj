#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>


int main(){
  auto a = new uint8_t[sizeof(aiScene)];
  auto model = aiImportFile("/home/dormon/Desktop/cube.obj",aiProcess_Triangulate|aiProcess_GenNormals|aiProcess_SortByPType);
  //std::memcpy(a,model,sizeof(aiScene));
  (aiScene&)*(aiScene*)a = *model;
  std::cout<<"mMeshes: "<<((aiScene*)a)->mNumMeshes<<std::endl;
  aiReleaseImport((decltype(model))a);
  delete[]a;
  return 0;
}





