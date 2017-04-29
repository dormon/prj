#pragma once

#include<geCore/Command.h>
#include<geGL/Export.h>
#include<GL/glew.h>
#include<geGL/AllAttribs.h>

namespace ge{
  namespace gl{
    template<
      typename BUF_TYPE      = GLuint,
      typename SRCRGB_TYPE   = GLenum,
      typename DSTRGB_TYPE   = GLenum,
      typename SRCALPHA_TYPE = GLenum,
      typename DSTALPHA_TYPE = GLenum>
    class BlendFuncSeparatei:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "buf"     )return(void*)&this->buf     ;
          if(name == "srcRGB"  )return(void*)&this->srcRGB  ;
          if(name == "dstRGB"  )return(void*)&this->dstRGB  ;
          if(name == "srcAlpha")return(void*)&this->srcAlpha;
          if(name == "dstAlpha")return(void*)&this->dstAlpha;
          return NULL;
        }
      public:
        BUF_TYPE      buf     ;
        SRCRGB_TYPE   srcRGB  ;
        DSTRGB_TYPE   dstRGB  ;
        SRCALPHA_TYPE srcAlpha;
        DSTALPHA_TYPE dstAlpha;
        BlendFuncSeparatei(
            BUF_TYPE      const&buf     ,
            SRCRGB_TYPE   const&srcRGB  ,
            DSTRGB_TYPE   const&dstRGB  ,
            SRCALPHA_TYPE const&srcAlpha,
            DSTALPHA_TYPE const&dstAlpha){
          this->buf      = buf     ;
          this->srcRGB   = srcRGB  ;
          this->dstRGB   = dstRGB  ;
          this->srcAlpha = srcAlpha;
          this->dstAlpha = dstAlpha;
        }
        virtual~BlendFuncSeparatei(){}
        virtual void operator()(){
          glBlendFuncSeparatei(
            ge::core::convertTo<GLuint>(this->buf     ),
            ge::core::convertTo<GLenum>(this->srcRGB  ),
            ge::core::convertTo<GLenum>(this->dstRGB  ),
            ge::core::convertTo<GLenum>(this->srcAlpha),
            ge::core::convertTo<GLenum>(this->dstAlpha)
          );
        }
    };

    template<
      typename MODE_TYPE = GLenum>
    class BlendEquation:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "mode")return(void*)&this->mode;
          return NULL;
        }
      public:
        MODE_TYPE mode;
        BlendEquation(
            MODE_TYPE const&mode){
          this->mode = mode;
        }
        virtual~BlendEquation(){}
        virtual void operator()(){
          glBlendEquation(
            ge::core::convertTo<GLenum>(this->mode)
          );
        }
    };

    template<
      typename SFACTORRGB_TYPE   = GLenum,
      typename DFACTORRGB_TYPE   = GLenum,
      typename SFACTORALPHA_TYPE = GLenum,
      typename DFACTORALPHA_TYPE = GLenum>
    class BlendFuncSeparate:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "sfactorRGB"  )return(void*)&this->sfactorRGB  ;
          if(name == "dfactorRGB"  )return(void*)&this->dfactorRGB  ;
          if(name == "sfactorAlpha")return(void*)&this->sfactorAlpha;
          if(name == "dfactorAlpha")return(void*)&this->dfactorAlpha;
          return NULL;
        }
      public:
        SFACTORRGB_TYPE   sfactorRGB  ;
        DFACTORRGB_TYPE   dfactorRGB  ;
        SFACTORALPHA_TYPE sfactorAlpha;
        DFACTORALPHA_TYPE dfactorAlpha;
        BlendFuncSeparate(
            SFACTORRGB_TYPE   const&sfactorRGB  ,
            DFACTORRGB_TYPE   const&dfactorRGB  ,
            SFACTORALPHA_TYPE const&sfactorAlpha,
            DFACTORALPHA_TYPE const&dfactorAlpha){
          this->sfactorRGB   = sfactorRGB  ;
          this->dfactorRGB   = dfactorRGB  ;
          this->sfactorAlpha = sfactorAlpha;
          this->dfactorAlpha = dfactorAlpha;
        }
        virtual~BlendFuncSeparate(){}
        virtual void operator()(){
          glBlendFuncSeparate(
            ge::core::convertTo<GLenum>(this->sfactorRGB  ),
            ge::core::convertTo<GLenum>(this->dfactorRGB  ),
            ge::core::convertTo<GLenum>(this->sfactorAlpha),
            ge::core::convertTo<GLenum>(this->dfactorAlpha)
          );
        }
    };

    template<
      typename SFACTOR_TYPE = GLenum,
      typename DFACTOR_TYPE = GLenum>
    class BlendFunc:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "sfactor")return(void*)&this->sfactor;
          if(name == "dfactor")return(void*)&this->dfactor;
          return NULL;
        }
      public:
        SFACTOR_TYPE sfactor;
        DFACTOR_TYPE dfactor;
        BlendFunc(
            SFACTOR_TYPE const&sfactor,
            DFACTOR_TYPE const&dfactor){
          this->sfactor = sfactor;
          this->dfactor = dfactor;
        }
        virtual~BlendFunc(){}
        virtual void operator()(){
          glBlendFunc(
            ge::core::convertTo<GLenum>(this->sfactor),
            ge::core::convertTo<GLenum>(this->dfactor)
          );
        }
    };

    template<
      typename BUF_TYPE  = GLuint,
      typename MODE_TYPE = GLenum>
    class BlendEquationi:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "buf" )return(void*)&this->buf ;
          if(name == "mode")return(void*)&this->mode;
          return NULL;
        }
      public:
        BUF_TYPE  buf ;
        MODE_TYPE mode;
        BlendEquationi(
            BUF_TYPE  const&buf ,
            MODE_TYPE const&mode){
          this->buf  = buf ;
          this->mode = mode;
        }
        virtual~BlendEquationi(){}
        virtual void operator()(){
          glBlendEquationi(
            ge::core::convertTo<GLuint>(this->buf ),
            ge::core::convertTo<GLenum>(this->mode)
          );
        }
    };

    template<
      typename RED_TYPE   = GLclampf,
      typename GREEN_TYPE = GLclampf,
      typename BLUE_TYPE  = GLclampf,
      typename ALPHA_TYPE = GLclampf>
    class BlendColor:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "red"  )return(void*)&this->red  ;
          if(name == "green")return(void*)&this->green;
          if(name == "blue" )return(void*)&this->blue ;
          if(name == "alpha")return(void*)&this->alpha;
          return NULL;
        }
      public:
        RED_TYPE   red  ;
        GREEN_TYPE green;
        BLUE_TYPE  blue ;
        ALPHA_TYPE alpha;
        BlendColor(
            RED_TYPE   const&red  ,
            GREEN_TYPE const&green,
            BLUE_TYPE  const&blue ,
            ALPHA_TYPE const&alpha){
          this->red   = red  ;
          this->green = green;
          this->blue  = blue ;
          this->alpha = alpha;
        }
        virtual~BlendColor(){}
        virtual void operator()(){
          glBlendColor(
            ge::core::convertTo<GLclampf>(this->red  ),
            ge::core::convertTo<GLclampf>(this->green),
            ge::core::convertTo<GLclampf>(this->blue ),
            ge::core::convertTo<GLclampf>(this->alpha)
          );
        }
    };

    template<
      typename MODERGB_TYPE   = GLenum,
      typename MODEALPHA_TYPE = GLenum>
    class BlendEquationSeparate:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "modeRGB"  )return(void*)&this->modeRGB  ;
          if(name == "modeAlpha")return(void*)&this->modeAlpha;
          return NULL;
        }
      public:
        MODERGB_TYPE   modeRGB  ;
        MODEALPHA_TYPE modeAlpha;
        BlendEquationSeparate(
            MODERGB_TYPE   const&modeRGB  ,
            MODEALPHA_TYPE const&modeAlpha){
          this->modeRGB   = modeRGB  ;
          this->modeAlpha = modeAlpha;
        }
        virtual~BlendEquationSeparate(){}
        virtual void operator()(){
          glBlendEquationSeparate(
            ge::core::convertTo<GLenum>(this->modeRGB  ),
            ge::core::convertTo<GLenum>(this->modeAlpha)
          );
        }
    };

    template<
      typename BUF_TYPE       = GLuint,
      typename MODERGB_TYPE   = GLenum,
      typename MODEALPHA_TYPE = GLenum>
    class BlendEquationSeparatei:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "buf"      )return(void*)&this->buf      ;
          if(name == "modeRGB"  )return(void*)&this->modeRGB  ;
          if(name == "modeAlpha")return(void*)&this->modeAlpha;
          return NULL;
        }
      public:
        BUF_TYPE       buf      ;
        MODERGB_TYPE   modeRGB  ;
        MODEALPHA_TYPE modeAlpha;
        BlendEquationSeparatei(
            BUF_TYPE       const&buf      ,
            MODERGB_TYPE   const&modeRGB  ,
            MODEALPHA_TYPE const&modeAlpha){
          this->buf       = buf      ;
          this->modeRGB   = modeRGB  ;
          this->modeAlpha = modeAlpha;
        }
        virtual~BlendEquationSeparatei(){}
        virtual void operator()(){
          glBlendEquationSeparatei(
            ge::core::convertTo<GLuint>(this->buf      ),
            ge::core::convertTo<GLenum>(this->modeRGB  ),
            ge::core::convertTo<GLenum>(this->modeAlpha)
          );
        }
    };

    template<
      typename BUF_TYPE = GLuint,
      typename SRC_TYPE = GLenum,
      typename DST_TYPE = GLenum>
    class BlendFunci:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "buf")return(void*)&this->buf;
          if(name == "src")return(void*)&this->src;
          if(name == "dst")return(void*)&this->dst;
          return NULL;
        }
      public:
        BUF_TYPE buf;
        SRC_TYPE src;
        DST_TYPE dst;
        BlendFunci(
            BUF_TYPE const&buf,
            SRC_TYPE const&src,
            DST_TYPE const&dst){
          this->buf = buf;
          this->src = src;
          this->dst = dst;
        }
        virtual~BlendFunci(){}
        virtual void operator()(){
          glBlendFunci(
            ge::core::convertTo<GLuint>(this->buf),
            ge::core::convertTo<GLenum>(this->src),
            ge::core::convertTo<GLenum>(this->dst)
          );
        }
    };

    template<
      typename RED_TYPE   = GLfloat,
      typename GREEN_TYPE = GLfloat,
      typename BLUE_TYPE  = GLfloat,
      typename ALPHA_TYPE = GLfloat>
    class BlendColor:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "red"  )return(void*)&this->red  ;
          if(name == "green")return(void*)&this->green;
          if(name == "blue" )return(void*)&this->blue ;
          if(name == "alpha")return(void*)&this->alpha;
          return NULL;
        }
      public:
        RED_TYPE   red  ;
        GREEN_TYPE green;
        BLUE_TYPE  blue ;
        ALPHA_TYPE alpha;
        BlendColor(
            RED_TYPE   const&red  ,
            GREEN_TYPE const&green,
            BLUE_TYPE  const&blue ,
            ALPHA_TYPE const&alpha){
          this->red   = red  ;
          this->green = green;
          this->blue  = blue ;
          this->alpha = alpha;
        }
        virtual~BlendColor(){}
        virtual void operator()(){
          glBlendColor(
            ge::core::convertTo<GLfloat>(this->red  ),
            ge::core::convertTo<GLfloat>(this->green),
            ge::core::convertTo<GLfloat>(this->blue ),
            ge::core::convertTo<GLfloat>(this->alpha)
          );
        }
    };

    template<
      typename BUF_TYPE      = GLuint,
      typename SRCRGB_TYPE   = GLenum,
      typename DSTRGB_TYPE   = GLenum,
      typename SRCALPHA_TYPE = GLenum,
      typename DSTALPHA_TYPE = GLenum>
    inline BlendFuncSeparatei<
      BUF_TYPE     ,
      SRCRGB_TYPE  ,
      DSTRGB_TYPE  ,
      SRCALPHA_TYPE,
      DSTALPHA_TYPE>* newBlendFuncSeparatei(
        BUF_TYPE      const&buf     ,
        SRCRGB_TYPE   const&srcRGB  ,
        DSTRGB_TYPE   const&dstRGB  ,
        SRCALPHA_TYPE const&srcAlpha,
        DSTALPHA_TYPE const&dstAlpha){
        return new BlendFuncSeparatei<
          BUF_TYPE     ,
          SRCRGB_TYPE  ,
          DSTRGB_TYPE  ,
          SRCALPHA_TYPE,
          DSTALPHA_TYPE>(
              buf     ,
              srcRGB  ,
              dstRGB  ,
              srcAlpha,
              dstAlpha);
    }
    template<
      typename MODE_TYPE = GLenum>
    inline BlendEquation<
      MODE_TYPE>* newBlendEquation(
        MODE_TYPE const&mode){
        return new BlendEquation<
          MODE_TYPE>(
              mode);
    }
    template<
      typename SFACTORRGB_TYPE   = GLenum,
      typename DFACTORRGB_TYPE   = GLenum,
      typename SFACTORALPHA_TYPE = GLenum,
      typename DFACTORALPHA_TYPE = GLenum>
    inline BlendFuncSeparate<
      SFACTORRGB_TYPE  ,
      DFACTORRGB_TYPE  ,
      SFACTORALPHA_TYPE,
      DFACTORALPHA_TYPE>* newBlendFuncSeparate(
        SFACTORRGB_TYPE   const&sfactorRGB  ,
        DFACTORRGB_TYPE   const&dfactorRGB  ,
        SFACTORALPHA_TYPE const&sfactorAlpha,
        DFACTORALPHA_TYPE const&dfactorAlpha){
        return new BlendFuncSeparate<
          SFACTORRGB_TYPE  ,
          DFACTORRGB_TYPE  ,
          SFACTORALPHA_TYPE,
          DFACTORALPHA_TYPE>(
              sfactorRGB  ,
              dfactorRGB  ,
              sfactorAlpha,
              dfactorAlpha);
    }
    template<
      typename SFACTOR_TYPE = GLenum,
      typename DFACTOR_TYPE = GLenum>
    inline BlendFunc<
      SFACTOR_TYPE,
      DFACTOR_TYPE>* newBlendFunc(
        SFACTOR_TYPE const&sfactor,
        DFACTOR_TYPE const&dfactor){
        return new BlendFunc<
          SFACTOR_TYPE,
          DFACTOR_TYPE>(
              sfactor,
              dfactor);
    }
    template<
      typename BUF_TYPE  = GLuint,
      typename MODE_TYPE = GLenum>
    inline BlendEquationi<
      BUF_TYPE ,
      MODE_TYPE>* newBlendEquationi(
        BUF_TYPE  const&buf ,
        MODE_TYPE const&mode){
        return new BlendEquationi<
          BUF_TYPE ,
          MODE_TYPE>(
              buf ,
              mode);
    }
    template<
      typename RED_TYPE   = GLclampf,
      typename GREEN_TYPE = GLclampf,
      typename BLUE_TYPE  = GLclampf,
      typename ALPHA_TYPE = GLclampf>
    inline BlendColor<
      RED_TYPE  ,
      GREEN_TYPE,
      BLUE_TYPE ,
      ALPHA_TYPE>* newBlendColor(
        RED_TYPE   const&red  ,
        GREEN_TYPE const&green,
        BLUE_TYPE  const&blue ,
        ALPHA_TYPE const&alpha){
        return new BlendColor<
          RED_TYPE  ,
          GREEN_TYPE,
          BLUE_TYPE ,
          ALPHA_TYPE>(
              red  ,
              green,
              blue ,
              alpha);
    }
    template<
      typename MODERGB_TYPE   = GLenum,
      typename MODEALPHA_TYPE = GLenum>
    inline BlendEquationSeparate<
      MODERGB_TYPE  ,
      MODEALPHA_TYPE>* newBlendEquationSeparate(
        MODERGB_TYPE   const&modeRGB  ,
        MODEALPHA_TYPE const&modeAlpha){
        return new BlendEquationSeparate<
          MODERGB_TYPE  ,
          MODEALPHA_TYPE>(
              modeRGB  ,
              modeAlpha);
    }
    template<
      typename BUF_TYPE       = GLuint,
      typename MODERGB_TYPE   = GLenum,
      typename MODEALPHA_TYPE = GLenum>
    inline BlendEquationSeparatei<
      BUF_TYPE      ,
      MODERGB_TYPE  ,
      MODEALPHA_TYPE>* newBlendEquationSeparatei(
        BUF_TYPE       const&buf      ,
        MODERGB_TYPE   const&modeRGB  ,
        MODEALPHA_TYPE const&modeAlpha){
        return new BlendEquationSeparatei<
          BUF_TYPE      ,
          MODERGB_TYPE  ,
          MODEALPHA_TYPE>(
              buf      ,
              modeRGB  ,
              modeAlpha);
    }
    template<
      typename BUF_TYPE = GLuint,
      typename SRC_TYPE = GLenum,
      typename DST_TYPE = GLenum>
    inline BlendFunci<
      BUF_TYPE,
      SRC_TYPE,
      DST_TYPE>* newBlendFunci(
        BUF_TYPE const&buf,
        SRC_TYPE const&src,
        DST_TYPE const&dst){
        return new BlendFunci<
          BUF_TYPE,
          SRC_TYPE,
          DST_TYPE>(
              buf,
              src,
              dst);
    }
    template<
      typename RED_TYPE   = GLfloat,
      typename GREEN_TYPE = GLfloat,
      typename BLUE_TYPE  = GLfloat,
      typename ALPHA_TYPE = GLfloat>
    inline BlendColor<
      RED_TYPE  ,
      GREEN_TYPE,
      BLUE_TYPE ,
      ALPHA_TYPE>* newBlendColor(
        RED_TYPE   const&red  ,
        GREEN_TYPE const&green,
        BLUE_TYPE  const&blue ,
        ALPHA_TYPE const&alpha){
        return new BlendColor<
          RED_TYPE  ,
          GREEN_TYPE,
          BLUE_TYPE ,
          ALPHA_TYPE>(
              red  ,
              green,
              blue ,
              alpha);
    }
  }
}
