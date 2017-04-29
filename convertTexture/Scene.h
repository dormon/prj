#pragma once

#include<iostream>
#include<vector>
#include<memory>
#include<glm/glm.hpp>
#include<geCore/dtemplates.h>

namespace ge{
  namespace db{
    class VertexAttribDataDescriptor{
      public:
        unsigned    components;
        enum Type{
          I8 ,
          I16,
          I32,
          I64,
          U8 ,
          U16,
          U32,
          U64,
          F32,
          F64
        }type;
        enum Semantic{
          POSITION,
          COORD   ,
          NORMAL  ,
          BINORMAL,
          TANGENT ,
          COLOR   
        }semantic;
        VertexAttribDataDescriptor(
            unsigned      components = 4       ,
            enum Type     type       = F32     ,
            enum Semantic semantic   = POSITION);
        unsigned size();
        std::string type2Str();
        std::string semantic2Str();
        std::string toStr();
    };

    class VertexAttrib{
      protected:
        const void*                _ptr           ;
        unsigned                   _stride        ;
        unsigned                   _offset        ;
        VertexAttribDataDescriptor _dataDescriptor;
      public:
        VertexAttrib(
            const void*ptr                               ,
            VertexAttribDataDescriptor dataDescriptor    ,
            unsigned   offset                         = 0,
            unsigned   stride                         = 0);
        const void*get(unsigned vertex);
        const void*getPtr();
        unsigned   getStride();
        unsigned   getOffset();
        VertexAttribDataDescriptor&getDataDescriptor();
        std::string toStr();
    };

    class MeshVertices{
      protected:
        std::vector<VertexAttrib>_attribs;
        unsigned                 _nofVertices;
      public:
        unsigned getNofVertices();
        MeshVertices(unsigned nofVertices);
        unsigned   getNofAttribs();
        unsigned   addVertexAttrib(VertexAttrib&attrib);
        const void*getAttrib(unsigned index,unsigned vertex);
        unsigned   getAttribSize(unsigned index);
        std::string toStr();
    };

    class MeshPrimitives{
      public:
        DEF_ENUM(Type,POINT,LINE,LINE_STRIP,LINE_LOOP,LINE_ADJACENCY,LINE_STRIP_ADJACENCY,TRIANGLE,TRIANGLE_STRIP,TRIANGLE_FAN,TRIANGLE_ADJACENCY,TRIANGLE_STRIP_ADJACENCY,PATCH);
        Type type;
        unsigned             nofVerticesPerPrimitive;
        bool                 indexed                ;
        std::vector<unsigned>indices                ;
        unsigned             restartIndex           ;
        bool                 enableRestart          ;
        unsigned             nofVertices               ;
        MeshPrimitives(
            unsigned  nofVertices                       ,
            enum Type type                    = TRIANGLE,
            unsigned  nofVerticesPerPrimitive = 3       );
        MeshPrimitives(
            unsigned              nofVertices                         ,
            enum Type             type                                ,
            unsigned              nofVerticesPerPrimitive             ,
            std::vector<unsigned>&indices                             ,
            bool                  enableRestart           = false     ,
            unsigned              restartIndex            = 0xffffffff);
        std::string toStr();
    };

    class MeshGeometry{
      public:
        void* positionData;
        void* normalData;

        MeshVertices*   vertices  ;
        MeshPrimitives* primitives;

        static void createxAttribData(
            MeshVertices*vertices,
            void**data,
            unsigned nofVertices,
            VertexAttribDataDescriptor dataDescriptor);

        MeshGeometry();
        ~MeshGeometry();
        std::string toStr();
    };

    class Mesh{
      public:
        std::shared_ptr<MeshGeometry>geometry;
        glm::mat4    matrix  ;
    };

    class Model{
      public:
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Model>>models;
        glm::mat4 matrix;
        ~Model();
    };

    class Scene{
      public:
        std::vector<std::shared_ptr<MeshGeometry>>geometries;
        std::shared_ptr<Model>root;
        ~Scene();
    };

    class ModelLoader{
      public:
        virtual ~ModelLoader();
        virtual std::shared_ptr<Scene>load(std::string file)=0;
    };

    class ModelLoaderManager{
      protected:
        std::vector<ModelLoader*>_loaders;
        std::vector<std::string >_loadersRegExps;
        bool _canLoad(std::string regexp,std::string file);
      public:
        void registerLoader(ModelLoader*loader,std::string regexp="");
        std::shared_ptr<Scene>load(std::string file);
    };
  }
}
