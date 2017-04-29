#pragma once

#include<geCore/Command.h>
#include<geGL/Export.h>
#include<GL/glew.h>
#include<geGL/AllAttribs.h>

namespace ge{
  namespace gl{
    template<
      typename INDEX_TYPE = GLuint      ,
      typename V_TYPE     = const GLint*>
    class ScissorIndexedv:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "index")return(void*)&this->index;
          if(name == "v"    )return(void*)&this->v    ;
          return NULL;
        }
      public:
        INDEX_TYPE index;
        V_TYPE     v    ;
        ScissorIndexedv(
            INDEX_TYPE const&index,
            V_TYPE     const&v    ){
          this->index = index;
          this->v     = v    ;
        }
        virtual~ScissorIndexedv(){}
        virtual void operator()(){
          glScissorIndexedv(
            ge::core::convertTo<GLuint      >(this->index),
            ge::core::convertTo<const GLint*>(this->v    )
          );
        }
    };

    template<
      typename X_TYPE      = GLint  ,
      typename Y_TYPE      = GLint  ,
      typename WIDTH_TYPE  = GLsizei,
      typename HEIGHT_TYPE = GLsizei>
    class Scissor:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "x"     )return(void*)&this->x     ;
          if(name == "y"     )return(void*)&this->y     ;
          if(name == "width" )return(void*)&this->width ;
          if(name == "height")return(void*)&this->height;
          return NULL;
        }
      public:
        X_TYPE      x     ;
        Y_TYPE      y     ;
        WIDTH_TYPE  width ;
        HEIGHT_TYPE height;
        Scissor(
            X_TYPE      const&x     ,
            Y_TYPE      const&y     ,
            WIDTH_TYPE  const&width ,
            HEIGHT_TYPE const&height){
          this->x      = x     ;
          this->y      = y     ;
          this->width  = width ;
          this->height = height;
        }
        virtual~Scissor(){}
        virtual void operator()(){
          glScissor(
            ge::core::convertTo<GLint  >(this->x     ),
            ge::core::convertTo<GLint  >(this->y     ),
            ge::core::convertTo<GLsizei>(this->width ),
            ge::core::convertTo<GLsizei>(this->height)
          );
        }
    };

    template<
      typename INDEX_TYPE  = GLuint ,
      typename LEFT_TYPE   = GLint  ,
      typename BOTTOM_TYPE = GLint  ,
      typename WIDTH_TYPE  = GLsizei,
      typename HEIGHT_TYPE = GLsizei>
    class ScissorIndexed:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "index" )return(void*)&this->index ;
          if(name == "left"  )return(void*)&this->left  ;
          if(name == "bottom")return(void*)&this->bottom;
          if(name == "width" )return(void*)&this->width ;
          if(name == "height")return(void*)&this->height;
          return NULL;
        }
      public:
        INDEX_TYPE  index ;
        LEFT_TYPE   left  ;
        BOTTOM_TYPE bottom;
        WIDTH_TYPE  width ;
        HEIGHT_TYPE height;
        ScissorIndexed(
            INDEX_TYPE  const&index ,
            LEFT_TYPE   const&left  ,
            BOTTOM_TYPE const&bottom,
            WIDTH_TYPE  const&width ,
            HEIGHT_TYPE const&height){
          this->index  = index ;
          this->left   = left  ;
          this->bottom = bottom;
          this->width  = width ;
          this->height = height;
        }
        virtual~ScissorIndexed(){}
        virtual void operator()(){
          glScissorIndexed(
            ge::core::convertTo<GLuint >(this->index ),
            ge::core::convertTo<GLint  >(this->left  ),
            ge::core::convertTo<GLint  >(this->bottom),
            ge::core::convertTo<GLsizei>(this->width ),
            ge::core::convertTo<GLsizei>(this->height)
          );
        }
    };

    template<
      typename FIRST_TYPE = GLuint      ,
      typename COUNT_TYPE = GLsizei     ,
      typename V_TYPE     = const GLint*>
    class ScissorArrayv:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "first")return(void*)&this->first;
          if(name == "count")return(void*)&this->count;
          if(name == "v"    )return(void*)&this->v    ;
          return NULL;
        }
      public:
        FIRST_TYPE first;
        COUNT_TYPE count;
        V_TYPE     v    ;
        ScissorArrayv(
            FIRST_TYPE const&first,
            COUNT_TYPE const&count,
            V_TYPE     const&v    ){
          this->first = first;
          this->count = count;
          this->v     = v    ;
        }
        virtual~ScissorArrayv(){}
        virtual void operator()(){
          glScissorArrayv(
            ge::core::convertTo<GLuint      >(this->first),
            ge::core::convertTo<GLsizei     >(this->count),
            ge::core::convertTo<const GLint*>(this->v    )
          );
        }
    };

    template<
      typename INDEX_TYPE = GLuint      ,
      typename V_TYPE     = const GLint*>
    inline ScissorIndexedv<
      INDEX_TYPE,
      V_TYPE    >* newScissorIndexedv(
        INDEX_TYPE const&index,
        V_TYPE     const&v    ){
        return new ScissorIndexedv<
          INDEX_TYPE,
          V_TYPE    >(
              index,
              v    );
    }
    template<
      typename X_TYPE      = GLint  ,
      typename Y_TYPE      = GLint  ,
      typename WIDTH_TYPE  = GLsizei,
      typename HEIGHT_TYPE = GLsizei>
    inline Scissor<
      X_TYPE     ,
      Y_TYPE     ,
      WIDTH_TYPE ,
      HEIGHT_TYPE>* newScissor(
        X_TYPE      const&x     ,
        Y_TYPE      const&y     ,
        WIDTH_TYPE  const&width ,
        HEIGHT_TYPE const&height){
        return new Scissor<
          X_TYPE     ,
          Y_TYPE     ,
          WIDTH_TYPE ,
          HEIGHT_TYPE>(
              x     ,
              y     ,
              width ,
              height);
    }
    template<
      typename INDEX_TYPE  = GLuint ,
      typename LEFT_TYPE   = GLint  ,
      typename BOTTOM_TYPE = GLint  ,
      typename WIDTH_TYPE  = GLsizei,
      typename HEIGHT_TYPE = GLsizei>
    inline ScissorIndexed<
      INDEX_TYPE ,
      LEFT_TYPE  ,
      BOTTOM_TYPE,
      WIDTH_TYPE ,
      HEIGHT_TYPE>* newScissorIndexed(
        INDEX_TYPE  const&index ,
        LEFT_TYPE   const&left  ,
        BOTTOM_TYPE const&bottom,
        WIDTH_TYPE  const&width ,
        HEIGHT_TYPE const&height){
        return new ScissorIndexed<
          INDEX_TYPE ,
          LEFT_TYPE  ,
          BOTTOM_TYPE,
          WIDTH_TYPE ,
          HEIGHT_TYPE>(
              index ,
              left  ,
              bottom,
              width ,
              height);
    }
    template<
      typename FIRST_TYPE = GLuint      ,
      typename COUNT_TYPE = GLsizei     ,
      typename V_TYPE     = const GLint*>
    inline ScissorArrayv<
      FIRST_TYPE,
      COUNT_TYPE,
      V_TYPE    >* newScissorArrayv(
        FIRST_TYPE const&first,
        COUNT_TYPE const&count,
        V_TYPE     const&v    ){
        return new ScissorArrayv<
          FIRST_TYPE,
          COUNT_TYPE,
          V_TYPE    >(
              first,
              count,
              v    );
    }
  }
}
