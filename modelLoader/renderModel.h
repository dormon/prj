#pragma once

#include"Scene.h"
#include<geGL/geGL.h>

class RenderMesh{
  public:
    RenderMesh(std::shared_ptr<ge::db::Mesh>mesh);
    ~RenderMesh();
    ge::gl::VertexArrayObject*_vao;
    ge::gl::BufferObject*_verticesVBO;
    ge::gl::BufferObject*_normalsVBO;
    ge::gl::BufferObject*_elementsVBO;
    unsigned _nofElements;
    glm::mat4 _matrix;
    void draw(ge::gl::ProgramObject*program,glm::mat4 model);
};

class RenderModel{
  public:
    std::vector<std::shared_ptr<RenderMesh>> _renderMeshes;
    std::vector<std::shared_ptr<RenderModel>>_renderModels;
    glm::mat4 _matrix;
    RenderModel(std::shared_ptr<ge::db::Model>model);
    ~RenderModel();
    void draw(ge::gl::ProgramObject*program,glm::mat4 model);
};

class RenderScene{
  protected:
    std::map<std::shared_ptr<ge::db::Mesh>,std::shared_ptr<RenderMesh>>_mesh2RenderMesh;
    std::vector<std::shared_ptr<RenderMesh>>_renderMeshes;
    std::shared_ptr<RenderModel>_root;
    std::shared_ptr<RenderMesh>_createRenderMesh(std::shared_ptr<ge::db::Mesh>mesh);
    std::shared_ptr<RenderModel>_createRenderModel(std::shared_ptr<ge::db::Model>model,std::shared_ptr<ge::db::Scene>scene);
  public:
    RenderScene(std::shared_ptr<ge::db::Scene>scene);

    ~RenderScene();
    void draw(ge::gl::ProgramObject*program);
};

class AdvancedRenderScene{
  protected:
    ge::gl::VertexArrayObject*_vao;
    ge::gl::BufferObject*_verticesVBO;
    ge::gl::BufferObject*_normalsVBO ;
    ge::gl::BufferObject*_elementsVBO;
    ge::gl::BufferObject*_matricesVBO;
    ge::gl::BufferObject*_matrialVBO ;
    ge::gl::BufferObject*_dibo;
    unsigned _nofMeshes;
  public:
    AdvancedRenderScene(std::shared_ptr<ge::db::Scene>scene);
    ~AdvancedRenderScene();
    void draw();
};
