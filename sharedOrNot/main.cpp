#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
//#include<geGL/geGL.h>
#include<GL/glew.h>
#include"attrib.h"

class BufferObj{
  protected:
    GLuint*_id;
  public:
    BufferObj(GLuint id){this->_id=new GLuint[1];this->_id[0] = 0;}
    ~BufferObj(){delete[]this->_id;}
    inline void   setId(GLuint id){this->_id[0]=id;}
    inline GLuint getId(){return this->_id[0];}
};

template<typename TYPE>
inline TYPE convertTo(std::shared_ptr<TYPE> const&sharedPtr){
  return *sharedPtr;
}

template<typename TYPE>
inline TYPE convertTo(TYPE*const&ptr){
  return *ptr;
}

template<typename TYPE>
inline TYPE convertTo(TYPE const&val){
  return val;
}

template<typename TYPE>
inline TYPE convertTo(std::shared_ptr<BufferObj>const&sharedBufferObj){
  return sharedBufferObj->getId();
}


template<typename TYPE>
inline void convertFrom(std::shared_ptr<TYPE>&output,TYPE const&input){
  *output=input;
}

template<typename TYPE>
inline void convertFrom(TYPE*&output,TYPE const&input){
  *output=input;
}

template<typename TYPE>
inline void convertFrom(TYPE &output,TYPE const&input){
  output = input;
}


DEF_NAMED_TEMPLATE_ATTRIBUTE(Id);

template<typename TYPE = GLuint>
class UseProgram: public Id<TYPE>{
  public:
    UseProgram(TYPE const&id):Id<TYPE>(id){}
    virtual~UseProgram(){}
    virtual void operator()(){
      glUseProgram(convertTo<GLuint>(this->Id<TYPE>::_value));
    }
};

DEF_NAMED_TEMPLATE_ATTRIBUTE(Ptr   );
DEF_NAMED_TEMPLATE_ATTRIBUTE(Target);
DEF_NAMED_TEMPLATE_ATTRIBUTE(Access);
DEF_NAMED_TEMPLATE_ATTRIBUTE(Buffer);

template<typename PTR=GLvoid*,typename TARGET=GLenum,typename ACCESS=GLenum>
class MapBuffer: public Ptr<PTR>,Target<TARGET>,Access<ACCESS>{
  public:
    MapBuffer(PTR const&ptr,TARGET const&target,ACCESS const&access):
      Ptr<PTR>(ptr),Target<TARGET>(target),Access<ACCESS>(access){}
    virtual~MapBuffer(){}
    virtual void operator()(){
      convertFrom<GLvoid*>(this->Ptr<PTR>::_value,glMapBuffer(
            convertTo<GLenum>(this->Target<TARGET>::_value),
            convertTo<GLenum>(this->Access<ACCESS>::_value)));
    }
};



DEF_NAMED_TEMPLATE_ATTRIBUTE(N);
DEF_NAMED_TEMPLATE_ATTRIBUTE(Ids);

template<typename N_TYPE=GLsizei,typename IDS_TYPE=const GLuint*>
class DeleteQueries: public N<N_TYPE>,Ids<IDS_TYPE>{
  public:
    DeleteQueries(N_TYPE const&n,IDS_TYPE const&ids):
      N<N_TYPE>(n),Ids<IDS_TYPE>(ids){}
    virtual~DeleteQueries(){}
    virtual void operator()(){
      glDeleteQueries(
          convertTo<GLsizei      >(this->N  <N_TYPE  >::_value),
          convertTo<const GLuint*>(this->Ids<IDS_TYPE>::_value));
    }
};

/*
template<typename BUFFER=GLuint,typename TARGET=GLenum>
class BindBuffer: public Buffer<BUFFER>,Target<TARGET>{
  public:
    BindBuffer(BUFFER const&buffer,TARGET const&target):
      Buffer<BUFFER>(buffer),Target<TARGET>(target){}
    virtual~BindBuffer(){}
    virtual void operator()(){
      glBindBuffer(
          convertTo<GLenum>(this->Target<TARGET>::_value),
          convertTo<GLuint>(this->Buffer<BUFFER>::_value));
    }
};
// */

//*
template<typename BUFFER=GLuint,typename TARGET=GLenum>
class BindBuffer{
  protected:
    BUFFER _buffer;
    TARGET _target;
  public:
    BindBuffer(BUFFER const&buffer,TARGET const&target){
      this->_buffer = buffer;
      this->_target = target;
    }
    virtual~BindBuffer(){}
    virtual void operator()(){
      glBindBuffer(
          convertTo<GLenum>(this->_target),
          convertTo<GLuint>(this->_buffer));
    }
};
// */


template<typename T=float>
void pri(T t){
  std::cout<<t<<std::endl;
}

template<typename T=float>
class Moje{
  protected:
    T _value;
  public:
    Moje(T val){this->_value=val;}
};

class Em{
  public:
    int asd;
    Em(){this->asd=0;}
    Em(int v){this->asd=v;}
};

template<typename T=float>
Moje<T>*fac(T val){return new Moje<T>(val);}

int main(){
  delete new Moje<Em>(12);
  delete new Moje(12);
  delete fac(12);
  pri(12.3f);
  UseProgram<GLuint>*                 cmd       = new UseProgram<GLuint>(23);
  UseProgram<GLuint*>*                cmdPtr    = new UseProgram<GLuint*>(NULL);
  UseProgram<std::shared_ptr<GLuint>>*cmdShared = new UseProgram<std::shared_ptr<GLuint>>(std::make_shared<GLuint>(2));
  delete cmd;
  delete cmdPtr;
  delete cmdShared;

  GLvoid*ptr=NULL;
  delete new MapBuffer<GLvoid*,GLenum,GLenum>(NULL,GL_SHADER_STORAGE_BUFFER,GL_READ_WRITE);
  delete new MapBuffer<GLvoid**,GLenum,GLenum>(&ptr,GL_SHADER_STORAGE_BUFFER,GL_READ_WRITE);
  delete new MapBuffer<std::shared_ptr<GLvoid*>,GLenum,GLenum>(std::make_shared<GLvoid*>(&ptr),GL_SHADER_STORAGE_BUFFER,GL_READ_WRITE);
  
  delete new BindBuffer<GLuint,GLenum>(0,GL_ARRAY_BUFFER);
  delete new BindBuffer<GLuint,GLenum>(0,GL_ARRAY_BUFFER);
  delete new BindBuffer<std::shared_ptr<GLuint>,std::shared_ptr<GLenum>>(std::make_shared<GLuint>(10),std::make_shared<GLenum>(GL_ELEMENT_ARRAY_BUFFER));

  delete new BindBuffer<std::shared_ptr<BufferObj>,GLenum>(std::make_shared<BufferObj>(10),GL_ARRAY_BUFFER);

  delete new DeleteQueries<GLsizei,const GLuint*>(1,NULL);
  return 0;
}





