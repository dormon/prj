#pragma once

#include<geCore/Command.h>
#include<geGL/Export.h>
#include<GL/glew.h>
#include<geGL/AllAttribs.h>

namespace ge{
  namespace gl{
    template<
      typename TARGET_TYPE = GLenum,
      typename ID_TYPE     = GLuint>
    class BeginQuery:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "target")return(void*)&this->target;
          if(name == "id"    )return(void*)&this->id    ;
          return NULL;
        }
      public:
        TARGET_TYPE target;
        ID_TYPE     id    ;
        BeginQuery(
            TARGET_TYPE const&target,
            ID_TYPE     const&id    ){
          this->target = target;
          this->id     = id    ;
        }
        virtual~BeginQuery(){}
        virtual void operator()(){
          glBeginQuery(
            ge::core::convertTo<GLenum>(this->target),
            ge::core::convertTo<GLuint>(this->id    )
          );
        }
    };

    template<
      typename TARGET_TYPE = GLenum>
    class EndQuery:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "target")return(void*)&this->target;
          return NULL;
        }
      public:
        TARGET_TYPE target;
        EndQuery(
            TARGET_TYPE const&target){
          this->target = target;
        }
        virtual~EndQuery(){}
        virtual void operator()(){
          glEndQuery(
            ge::core::convertTo<GLenum>(this->target)
          );
        }
    };

    template<
      typename TARGET_TYPE = GLenum,
      typename INDEX_TYPE  = GLuint>
    class EndQueryIndexed:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "target")return(void*)&this->target;
          if(name == "index" )return(void*)&this->index ;
          return NULL;
        }
      public:
        TARGET_TYPE target;
        INDEX_TYPE  index ;
        EndQueryIndexed(
            TARGET_TYPE const&target,
            INDEX_TYPE  const&index ){
          this->target = target;
          this->index  = index ;
        }
        virtual~EndQueryIndexed(){}
        virtual void operator()(){
          glEndQueryIndexed(
            ge::core::convertTo<GLenum>(this->target),
            ge::core::convertTo<GLuint>(this->index )
          );
        }
    };

    template<
      typename TARGET_TYPE = GLenum,
      typename INDEX_TYPE  = GLuint,
      typename ID_TYPE     = GLuint>
    class BeginQueryIndexed:
        public ge::core::Command{
      protected:
        void*_getAttribAddress(std::string name){
          if(name == "target")return(void*)&this->target;
          if(name == "index" )return(void*)&this->index ;
          if(name == "id"    )return(void*)&this->id    ;
          return NULL;
        }
      public:
        TARGET_TYPE target;
        INDEX_TYPE  index ;
        ID_TYPE     id    ;
        BeginQueryIndexed(
            TARGET_TYPE const&target,
            INDEX_TYPE  const&index ,
            ID_TYPE     const&id    ){
          this->target = target;
          this->index  = index ;
          this->id     = id    ;
        }
        virtual~BeginQueryIndexed(){}
        virtual void operator()(){
          glBeginQueryIndexed(
            ge::core::convertTo<GLenum>(this->target),
            ge::core::convertTo<GLuint>(this->index ),
            ge::core::convertTo<GLuint>(this->id    )
          );
        }
    };

    template<
      typename TARGET_TYPE = GLenum,
      typename ID_TYPE     = GLuint>
    inline BeginQuery<
      TARGET_TYPE,
      ID_TYPE    >* newBeginQuery(
        TARGET_TYPE const&target,
        ID_TYPE     const&id    ){
        return new BeginQuery<
          TARGET_TYPE,
          ID_TYPE    >(
              target,
              id    );
    }
    template<
      typename TARGET_TYPE = GLenum>
    inline EndQuery<
      TARGET_TYPE>* newEndQuery(
        TARGET_TYPE const&target){
        return new EndQuery<
          TARGET_TYPE>(
              target);
    }
    template<
      typename TARGET_TYPE = GLenum,
      typename INDEX_TYPE  = GLuint>
    inline EndQueryIndexed<
      TARGET_TYPE,
      INDEX_TYPE >* newEndQueryIndexed(
        TARGET_TYPE const&target,
        INDEX_TYPE  const&index ){
        return new EndQueryIndexed<
          TARGET_TYPE,
          INDEX_TYPE >(
              target,
              index );
    }
    template<
      typename TARGET_TYPE = GLenum,
      typename INDEX_TYPE  = GLuint,
      typename ID_TYPE     = GLuint>
    inline BeginQueryIndexed<
      TARGET_TYPE,
      INDEX_TYPE ,
      ID_TYPE    >* newBeginQueryIndexed(
        TARGET_TYPE const&target,
        INDEX_TYPE  const&index ,
        ID_TYPE     const&id    ){
        return new BeginQueryIndexed<
          TARGET_TYPE,
          INDEX_TYPE ,
          ID_TYPE    >(
              target,
              index ,
              id    );
    }
  }
}
