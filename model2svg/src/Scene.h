#pragma once

#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<glm/glm.hpp>
#include<vector>
#include<memory>
#include<functional>

class MeshData{
  public:
    MeshData(){}
    void load(aiMesh*mesh){
      for(size_t v=0;v<mesh->mNumVertices;++v){
        auto const vert = mesh->mVertices[v];
        auto const glmvec = glm::vec3(vert.x,vert.y,vert.z);
        vertices.push_back(glmvec);
      }
      for(size_t f=0;f<mesh->mNumFaces;++f){
        auto const face = mesh->mFaces[f];
        if(face.mNumIndices == 3){
          triangles.push_back(glm::uvec3(face.mIndices[0],face.mIndices[1],face.mIndices[2]));
        }
      }
    }
    MeshData(aiMesh*mesh){
      load(mesh);
    }
    std::vector<glm::vec3>vertices;
    std::vector<glm::uvec3>triangles;
};

class Node{
  public:
    Node(){}
    Node(aiNode*n,std::vector<std::shared_ptr<MeshData>>const&meshes){
      load(n,meshes);
    }
    void load(aiNode*n,std::vector<std::shared_ptr<MeshData>>const&ms){
      for(size_t m=0;m<n->mNumMeshes;++n)
        meshes.emplace_back(ms[n->mMeshes[m]]);
      for(size_t i=0;i<16;++i)
        transformation[i/4][i%4] = n->mTransformation[0][i];
      if(glm::mat4(0.f) == transformation)
        transformation = glm::mat4(1.f);
      for(size_t c=0;c<n->mNumChildren;++c)
        children.emplace_back(std::make_shared<Node>(n->mChildren[c],ms));
    }
    void visitor(std::function<void(std::shared_ptr<Node>const&,glm::mat4 const&)>const&f,glm::mat4 const&tt)const{
      for(auto const&c:children){
        f(c,tt);
        c->visitor(f,tt*transformation);
      }
    }

    glm::mat4 transformation;
    std::vector<std::shared_ptr<MeshData>>meshes;
    std::vector<std::shared_ptr<Node>>children;
};

class Scene{
  public:
    Scene(){}
    void load(aiScene const*scene){
      if(scene == nullptr)throw std::runtime_error("scene is nullptr");
      for(size_t i=0;i<scene->mNumMeshes;++i){
        auto const mesh = scene->mMeshes[i];
        meshes.emplace_back(std::make_shared<MeshData>(mesh));
      }
      root = std::make_shared<Node>(scene->mRootNode,meshes);
    }
    Scene(aiScene const*scene){
      load(scene);
    }
    void visitor(std::function<void(std::shared_ptr<Node>const&,glm::mat4 const&tt)>const&f,glm::mat4 const&tt)const{
      f(root,tt);
      root->visitor(f,tt);
    }
    std::shared_ptr<Node>root;
    std::vector<std::shared_ptr<MeshData>>meshes;
};
