#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>

#include<geGL/geGL.h>
#include<geGL/OpenGLCommands.h>

class SecretClass { public: int i;} a;
int b=3;

template<typename T>
class MyTemplate {
  public:
    int i,j;
    MyTemplate() {
      i=ge::core::convertTo<int>(a);
      j=ge::core::convertTo<int>(b);
    }
};

namespace ge {
  namespace core {
    template<typename TYPE>
      inline TYPE convertTo(SecretClass& sc){
        return sc.i;
      }
    /*
    template<>unsigned convertTo(int const& a){
      return a;
    }
    */
    /*
    template<>const void*convertTo(decltype(nullptr)const&a){
      return a;
    }*/
    /*
    template<>long convertTo(int const& a){
      return a;
    }*/
/*
    template<typename TO,typename FROM>typename std::enable_if<std::is_integral<TO>::value,TO>::type convertTo(
        typename std::enable_if<std::is_integral<FROM>::value,FROM>::type const&a){
      return a;
    }
    */
  }
} 


//unsigned int ge::core::convertTo<unsigned int, int>(int const&)
//void const* ge::core::convertTo<void const*, decltype(nullptr)>(decltype(nullptr) const&)
//long ge::core::convertTo<long, int>(int const&)


int main(){
  ge::gl::sharedBindTexture(GL_TEXTURE_2D,0);//std::make_shared<ge::gl::TextureObject>(GL_TEXTURE_1D,GL_RGBA32F,0,1024));
  ge::gl::sharedBindTexture(GL_TEXTURE_2D,std::make_shared<ge::gl::TextureObject>(GL_TEXTURE_1D,GL_RGBA32F,0,1024));
  ge::gl::sharedBufferData(GL_ARRAY_BUFFER,4,nullptr,GL_STATIC_DRAW);
  return 0;
}
