#include<iostream>
#include<set>
#include<geCore/dtemplates.h>

#define GL_TEXTURE_BUFFER        123u
#define GL_ARRAY_BUFFER          21312u
#define GL_SHADER_STORAGE_BUFFER 2444u

#define GL_TEXTURE_BUFFER_BINDING        3212u
#define GL_ARRAY_BUFFER_BINDING          212123u
#define GL_SHADER_STORAGE_BUFFER_BINDING 3300u

unsigned translateBinding2Buffer(unsigned binding){
  switch(binding){
    case GL_TEXTURE_BUFFER_BINDING       :return GL_TEXTURE_BUFFER       ;
    case GL_ARRAY_BUFFER_BINDING         :return GL_ARRAY_BUFFER         ;
    case GL_SHADER_STORAGE_BUFFER_BINDING:return GL_SHADER_STORAGE_BUFFER;
    default                              :return ge::core::nonof(GL_TEXTURE_BUFFER,GL_ARRAY_BUFFER,GL_SHADER_STORAGE_BUFFER);
  }
}

unsigned translateBuffer2Binding(unsigned buffer){
  switch(buffer){
    case GL_TEXTURE_BUFFER       :return GL_TEXTURE_BUFFER_BINDING       ;
    case GL_ARRAY_BUFFER         :return GL_ARRAY_BUFFER_BINDING         ;
    case GL_SHADER_STORAGE_BUFFER:return GL_SHADER_STORAGE_BUFFER_BINDING;
    default                              :return ge::core::nonof(GL_TEXTURE_BUFFER_BINDING,GL_ARRAY_BUFFER_BINDING,GL_SHADER_STORAGE_BUFFER_BINDING);
  }
}


/*
template<typename TO,typename FROM>
TO translate(FROM from){
  return 0;
}

template<typename TO,typename FROM,TO FIRST_TO,FROM FIRST_FROM,typename... ARGS>
TO translate(FROM from){
  if(FIRST_FROM == from)return FIRST_TO;
  else translate<TO,FROM,ARGS...>(from);
}
*/

template<typename TO,typename FROM>
inline TO r_translate(FROM from){
  return 0;
}

template<typename TO,typename FROM,typename... ARGS>
inline TO r_translate(FROM from,TO CASE_TO,FROM CASE_FROM,ARGS... args){
  if(from == CASE_FROM)return CASE_TO;
  else return r_translate<TO,FROM>(from,args...);
}

template<typename TO,typename FROM>
TO translate(FROM from){
  return r_translate<TO,FROM>(from,
      GL_TEXTURE_BUFFER_BINDING       ,GL_TEXTURE_BUFFER       ,
      GL_ARRAY_BUFFER_BINDING         ,GL_ARRAY_BUFFER         ,
      GL_SHADER_STORAGE_BUFFER_BINDING,GL_SHADER_STORAGE_BUFFER);
}

int main(){
  std::cerr<<translate<unsigned,unsigned>(GL_SHADER_STORAGE_BUFFER)<<std::endl;
  /*
  translate<unsigned,unsigned,
    GL_TEXTURE_BUFFER_BINDING       ,GL_TEXTURE_BUFFER       ,
    GL_ARRAY_BUFFER_BINDING         ,GL_ARRAY_BUFFER         ,
    GL_SHADER_STORAGE_BUFFER_BINDING,GL_SHADER_STORAGE_BUFFER>(GL_SHADER_STORAGE_BUFFER_BINDING);
  */

  return 0;
}
