#pragma once

#include<geCore/Command.h>
#include<geGL/Export.h>
#include<GL/glew.h>
#include<geGL/AllAttribs.h>

namespace ge{
  namespace gl{
    template<
      typename FACE_TYPE = GLenum,
      typename FUNC_TYPE = GLenum,
      typename REF_TYPE  = GLint ,
      typename MASK_TYPE = GLuint>
    class StencilFuncSeparate:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "face")return(void*)&this->face;
          if(name == "func")return(void*)&this->func;
          if(name == "ref" )return(void*)&this->ref ;
          if(name == "mask")return(void*)&this->mask;
          return NULL;
        }
      public:
        FACE_TYPE face;
        FUNC_TYPE func;
        REF_TYPE  ref ;
        MASK_TYPE mask;
        StencilFuncSeparate(
            FACE_TYPE const&face,
            FUNC_TYPE const&func,
            REF_TYPE  const&ref ,
            MASK_TYPE const&mask){
          this->face = face;
          this->func = func;
          this->ref  = ref ;
          this->mask = mask;
        }
        virtual~StencilFuncSeparate(){}
        virtual void operator()(){
          glStencilFuncSeparate(
            ge::core::convertTo<GLenum>(this->face),
            ge::core::convertTo<GLenum>(this->func),
            ge::core::convertTo<GLint >(this->ref ),
            ge::core::convertTo<GLuint>(this->mask)
          );
        }
    };

    template<
      typename FACE_TYPE   = GLenum,
      typename SFAIL_TYPE  = GLenum,
      typename DPFAIL_TYPE = GLenum,
      typename DPPASS_TYPE = GLenum>
    class StencilOpSeparate:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "face"  )return(void*)&this->face  ;
          if(name == "sfail" )return(void*)&this->sfail ;
          if(name == "dpfail")return(void*)&this->dpfail;
          if(name == "dppass")return(void*)&this->dppass;
          return NULL;
        }
      public:
        FACE_TYPE   face  ;
        SFAIL_TYPE  sfail ;
        DPFAIL_TYPE dpfail;
        DPPASS_TYPE dppass;
        StencilOpSeparate(
            FACE_TYPE   const&face  ,
            SFAIL_TYPE  const&sfail ,
            DPFAIL_TYPE const&dpfail,
            DPPASS_TYPE const&dppass){
          this->face   = face  ;
          this->sfail  = sfail ;
          this->dpfail = dpfail;
          this->dppass = dppass;
        }
        virtual~StencilOpSeparate(){}
        virtual void operator()(){
          glStencilOpSeparate(
            ge::core::convertTo<GLenum>(this->face  ),
            ge::core::convertTo<GLenum>(this->sfail ),
            ge::core::convertTo<GLenum>(this->dpfail),
            ge::core::convertTo<GLenum>(this->dppass)
          );
        }
    };

    template<
      typename FUNC_TYPE = GLenum,
      typename REF_TYPE  = GLint ,
      typename MASK_TYPE = GLuint>
    class StencilFunc:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "func")return(void*)&this->func;
          if(name == "ref" )return(void*)&this->ref ;
          if(name == "mask")return(void*)&this->mask;
          return NULL;
        }
      public:
        FUNC_TYPE func;
        REF_TYPE  ref ;
        MASK_TYPE mask;
        StencilFunc(
            FUNC_TYPE const&func,
            REF_TYPE  const&ref ,
            MASK_TYPE const&mask){
          this->func = func;
          this->ref  = ref ;
          this->mask = mask;
        }
        virtual~StencilFunc(){}
        virtual void operator()(){
          glStencilFunc(
            ge::core::convertTo<GLenum>(this->func),
            ge::core::convertTo<GLint >(this->ref ),
            ge::core::convertTo<GLuint>(this->mask)
          );
        }
    };

    template<
      typename FAIL_TYPE  = GLenum,
      typename ZFAIL_TYPE = GLenum,
      typename ZPASS_TYPE = GLenum>
    class StencilOp:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "fail" )return(void*)&this->fail ;
          if(name == "zfail")return(void*)&this->zfail;
          if(name == "zpass")return(void*)&this->zpass;
          return NULL;
        }
      public:
        FAIL_TYPE  fail ;
        ZFAIL_TYPE zfail;
        ZPASS_TYPE zpass;
        StencilOp(
            FAIL_TYPE  const&fail ,
            ZFAIL_TYPE const&zfail,
            ZPASS_TYPE const&zpass){
          this->fail  = fail ;
          this->zfail = zfail;
          this->zpass = zpass;
        }
        virtual~StencilOp(){}
        virtual void operator()(){
          glStencilOp(
            ge::core::convertTo<GLenum>(this->fail ),
            ge::core::convertTo<GLenum>(this->zfail),
            ge::core::convertTo<GLenum>(this->zpass)
          );
        }
    };

    template<
      typename FACE_TYPE = GLenum,
      typename FUNC_TYPE = GLenum,
      typename REF_TYPE  = GLint ,
      typename MASK_TYPE = GLuint>
    inline StencilFuncSeparate<
      FACE_TYPE,
      FUNC_TYPE,
      REF_TYPE ,
      MASK_TYPE>* newStencilFuncSeparate(
        FACE_TYPE const&face,
        FUNC_TYPE const&func,
        REF_TYPE  const&ref ,
        MASK_TYPE const&mask){
        return new StencilFuncSeparate<
          FACE_TYPE,
          FUNC_TYPE,
          REF_TYPE ,
          MASK_TYPE>(
              face,
              func,
              ref ,
              mask);
    }
    template<
      typename FACE_TYPE   = GLenum,
      typename SFAIL_TYPE  = GLenum,
      typename DPFAIL_TYPE = GLenum,
      typename DPPASS_TYPE = GLenum>
    inline StencilOpSeparate<
      FACE_TYPE  ,
      SFAIL_TYPE ,
      DPFAIL_TYPE,
      DPPASS_TYPE>* newStencilOpSeparate(
        FACE_TYPE   const&face  ,
        SFAIL_TYPE  const&sfail ,
        DPFAIL_TYPE const&dpfail,
        DPPASS_TYPE const&dppass){
        return new StencilOpSeparate<
          FACE_TYPE  ,
          SFAIL_TYPE ,
          DPFAIL_TYPE,
          DPPASS_TYPE>(
              face  ,
              sfail ,
              dpfail,
              dppass);
    }
    template<
      typename FUNC_TYPE = GLenum,
      typename REF_TYPE  = GLint ,
      typename MASK_TYPE = GLuint>
    inline StencilFunc<
      FUNC_TYPE,
      REF_TYPE ,
      MASK_TYPE>* newStencilFunc(
        FUNC_TYPE const&func,
        REF_TYPE  const&ref ,
        MASK_TYPE const&mask){
        return new StencilFunc<
          FUNC_TYPE,
          REF_TYPE ,
          MASK_TYPE>(
              func,
              ref ,
              mask);
    }
    template<
      typename FAIL_TYPE  = GLenum,
      typename ZFAIL_TYPE = GLenum,
      typename ZPASS_TYPE = GLenum>
    inline StencilOp<
      FAIL_TYPE ,
      ZFAIL_TYPE,
      ZPASS_TYPE>* newStencilOp(
        FAIL_TYPE  const&fail ,
        ZFAIL_TYPE const&zfail,
        ZPASS_TYPE const&zpass){
        return new StencilOp<
          FAIL_TYPE ,
          ZFAIL_TYPE,
          ZPASS_TYPE>(
              fail ,
              zfail,
              zpass);
    }
  }
}
