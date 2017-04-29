#pragma once

#include"Scene.h"

#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

namespace ge{
  namespace db{
    class AssimpLoader: public ModelLoader{
      protected:
        unsigned _parseFlags(std::string file);
      public:
        virtual std::shared_ptr<Scene>load(std::string file);
    };
  }
}

