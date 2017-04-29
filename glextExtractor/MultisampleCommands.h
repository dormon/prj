#pragma once

#include<geCore/Command.h>
#include<geGL/Export.h>
#include<GL/glew.h>
#include<geGL/AllAttribs.h>

namespace ge{
  namespace gl{
    template<
      typename MASKNUMBER_TYPE = GLuint    ,
      typename MASK_TYPE       = GLbitfield>
    class SampleMaski:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "maskNumber")return(void*)&this->maskNumber;
          if(name == "mask"      )return(void*)&this->mask      ;
          return NULL;
        }
      public:
        MASKNUMBER_TYPE maskNumber;
        MASK_TYPE       mask      ;
        SampleMaski(
            MASKNUMBER_TYPE const&maskNumber,
            MASK_TYPE       const&mask      ){
          this->maskNumber = maskNumber;
          this->mask       = mask      ;
        }
        virtual~SampleMaski(){}
        virtual void operator()(){
          glSampleMaski(
            ge::core::convertTo<GLuint    >(this->maskNumber),
            ge::core::convertTo<GLbitfield>(this->mask      )
          );
        }
    };

    template<
      typename VALUE_TYPE  = GLclampf ,
      typename INVERT_TYPE = GLboolean>
    class SampleCoverage:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "value" )return(void*)&this->value ;
          if(name == "invert")return(void*)&this->invert;
          return NULL;
        }
      public:
        VALUE_TYPE  value ;
        INVERT_TYPE invert;
        SampleCoverage(
            VALUE_TYPE  const&value ,
            INVERT_TYPE const&invert){
          this->value  = value ;
          this->invert = invert;
        }
        virtual~SampleCoverage(){}
        virtual void operator()(){
          glSampleCoverage(
            ge::core::convertTo<GLclampf >(this->value ),
            ge::core::convertTo<GLboolean>(this->invert)
          );
        }
    };

    template<
      typename MASKNUMBER_TYPE = GLuint    ,
      typename MASK_TYPE       = GLbitfield>
    inline SampleMaski<
      MASKNUMBER_TYPE,
      MASK_TYPE      >* newSampleMaski(
        MASKNUMBER_TYPE const&maskNumber,
        MASK_TYPE       const&mask      ){
        return new SampleMaski<
          MASKNUMBER_TYPE,
          MASK_TYPE      >(
              maskNumber,
              mask      );
    }
    template<
      typename VALUE_TYPE  = GLclampf ,
      typename INVERT_TYPE = GLboolean>
    inline SampleCoverage<
      VALUE_TYPE ,
      INVERT_TYPE>* newSampleCoverage(
        VALUE_TYPE  const&value ,
        INVERT_TYPE const&invert){
        return new SampleCoverage<
          VALUE_TYPE ,
          INVERT_TYPE>(
              value ,
              invert);
    }
  }
}
