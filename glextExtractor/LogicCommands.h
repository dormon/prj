#pragma once

#include<geCore/Command.h>
#include<geGL/Export.h>
#include<GL/glew.h>
#include<geGL/AllAttribs.h>

namespace ge{
  namespace gl{
    template<
      typename OPCODE_TYPE = GLenum>
    class LogicOp:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "opcode")return(void*)&this->opcode;
          return NULL;
        }
      public:
        OPCODE_TYPE opcode;
        LogicOp(
            OPCODE_TYPE const&opcode){
          this->opcode = opcode;
        }
        virtual~LogicOp(){}
        virtual void operator()(){
          glLogicOp(
            ge::core::convertTo<GLenum>(this->opcode)
          );
        }
    };

    template<
      typename OPCODE_TYPE = GLenum>
    inline LogicOp<
      OPCODE_TYPE>* newLogicOp(
        OPCODE_TYPE const&opcode){
        return new LogicOp<
          OPCODE_TYPE>(
              opcode);
    }
  }
}
