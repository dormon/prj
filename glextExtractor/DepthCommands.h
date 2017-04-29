#pragma once

#include<geCore/Command.h>
#include<geGL/Export.h>
#include<GL/glew.h>
#include<geGL/AllAttribs.h>

namespace ge{
  namespace gl{
    template<
      typename FUNC_TYPE = GLenum>
    class DepthFunc:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "func")return(void*)&this->func;
          return NULL;
        }
      public:
        FUNC_TYPE func;
        DepthFunc(
            FUNC_TYPE const&func){
          this->func = func;
        }
        virtual~DepthFunc(){}
        virtual void operator()(){
          glDepthFunc(
            ge::core::convertTo<GLenum>(this->func)
          );
        }
    };

    template<
      typename FUNC_TYPE = GLenum>
    inline DepthFunc<
      FUNC_TYPE>* newDepthFunc(
        FUNC_TYPE const&func){
        return new DepthFunc<
          FUNC_TYPE>(
              func);
    }
  }
}
