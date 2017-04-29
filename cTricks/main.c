#include<stdlib.h>
#include<stdio.h>

typedef struct Vec4{
  float data[4];
}Vec4;

typedef struct Mat4{
  Vec4 column[4];
}Mat4;

void multiply_Mat4_Vec4(
    Vec4      *output,
    Mat4 const*left  ,
    Vec4 const*right ){
  for(size_t y=0;y<4;++y){
    output->data[y] = 0;
    for(size_t x=0;x<4;++x)
      output->data[y] += left->column[x].data[y] * right->data[x];
  }
}

void multiply_Mat4_Mat4(
    Mat4      *output,
    Mat4 const*left  ,
    Mat4 const*right ){
  for(size_t c=0;c<4;++c)
    multiply_Mat4_Vec4(output->column + c, left, right->column + c);
}

typedef enum ComponentType{
  INT  ,
  UINT ,
  FLOAT,
}ComponentType;

enum InterpolationType{
  FLAT         ,
  NOPERSPECTIVE,
  SMOOTH       ,
};

typedef struct Data{
  void*data;

}Data;

#define MAX_VERTEX_ATTRIBUTES 16

typedef struct VertexAttributeDescription{
  ComponentType type         ;
  size_t        nofComponents;
  size_t        offset       ;
  size_t        stride       ;
  int           enabled      ;
  void const*   buffer       ;
}VertexAttributeDescription;

typedef struct VertexArrayObject{
  VertexAttributeDescription attributes[MAX_VERTEX_ATTRIBUTES];
}VertexArrayObject;

typedef struct Vertex{
  void const* attributes[MAX_VERTEX_ATTRIBUTES];
}Vertex;

typedef struct OutputVertex{
  void* attributes[MAX_VERTEX_ATTRIBUTES];
  Vec4  gl_Position;
}OutputVertex;


Vertex runVertexPuller(VertexArrayObject const*vao,size_t gl_VertexID){
  Vertex output;
  for(size_t i=0;i<MAX_VERTEX_ATTRIBUTES;++i){
    if(vao->attributes[i].enabled)
      output.attributes[i] = (unsigned char*)vao->attributes[i].buffer + vao->attributes[i].offset + gl_VertexID * vao->attributes[i].stride;
  }
  return output;
}

OutputVertex runVertexShader(Vertex const*vertex){
  OutputVertex output;
  Mat4 mvp;
  multiply_Mat4_Vec4(&output.gl_Position,&mvp,(Vec4 const*)vertex->attributes[0]);
  return output;
}
  
int main(){
  return 0;
}
