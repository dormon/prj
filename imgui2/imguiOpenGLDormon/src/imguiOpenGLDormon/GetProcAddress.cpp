#include <imguiOpenGLDormon/GetProcAddress.h>

#include <stdexcept>
#include <cstdint>
#include <iostream>

namespace imguiOpenGLDormon{

#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include<Windows.h>


class OpenGLFunctionLoader{
  protected:
    bool _triedToLoadOpenGL = false;
    bool _triedToLoadGetProcAddress = false;
    HMODULE _openglLib = nullptr;
    using PROC = int(*)();
    using WGLGETPROCADDRESS = PROC(__stdcall*)(LPCSTR);
    WGLGETPROCADDRESS _wglGetProcAddress = nullptr;
  public:
    void*operator()(char const*name){
      assert(this != nullptr);
      const std::string libName = "opengl32.dll";
      const std::string getProcAddressName = "wglGetProcAddress";
      if (!this->_triedToLoadOpenGL){
        this->_triedToLoadOpenGL = true;
        this->_openglLib = LoadLibrary(TEXT(libName.c_str()));
      }
      if (!this->_triedToLoadGetProcAddress){
        this->_triedToLoadGetProcAddress = true;
        if (this->_openglLib)
          this->_wglGetProcAddress = (WGLGETPROCADDRESS)GetProcAddress(this->_openglLib, TEXT(getProcAddressName.c_str()));
        else throw std::runtime_error(std::string("geGL::OpenGLFunctionLoader::operator() - cannot open ") + libName);
      }
      if (!this->_wglGetProcAddress){
        throw std::runtime_error(std::string("geGL::OpenGLFunctionLoader::operator() - cannot load ") + getProcAddressName);
        return nullptr;
      }
      auto ret = (void*)this->_wglGetProcAddress(name);
      if (ret) return ret;
      return (void*)GetProcAddress(this->_openglLib, TEXT(name));
    }
    ~OpenGLFunctionLoader(){
      if (this->_openglLib)FreeLibrary(this->_openglLib);
      this->_openglLib = nullptr;
    }
};
#else
#include<dlfcn.h>
class OpenGLFunctionLoader{
  protected:
    bool _triedToLoadOpenGL = false;
    bool _triedToLoadGetProcAddress = false;
    void*openglLib = nullptr;
    using PROC = void(*)();
    using GETPROCTYPE = PROC(*)(uint8_t const*);
    GETPROCTYPE _glXGetProcAddress = nullptr;
  public:
    void*operator()(char const*name){
      const std::string libName = "libGL.so.1";
      const std::string getProcAddressName = "glXGetProcAddress";
      if(!this->_triedToLoadOpenGL){
        this->_triedToLoadOpenGL = true;
        this->openglLib = dlopen(libName.c_str(),RTLD_LAZY);
      }
      if(!this->_triedToLoadGetProcAddress){
        this->_triedToLoadGetProcAddress = true;
        if(this->openglLib)
          reinterpret_cast<void*&>(this->_glXGetProcAddress) = dlsym(this->openglLib,getProcAddressName.c_str());
        else throw std::runtime_error("geGL::OpenGLFunctionLoader::operator() - cannot open "+libName);
      }
      if(!this->_glXGetProcAddress){
        throw std::runtime_error("geGL::OpenGLFunctionLoader::operator() - cannot load " + getProcAddressName);
        return nullptr;
      }
      return (void*)this->_glXGetProcAddress((uint8_t const*)(name));
    }
    ~OpenGLFunctionLoader(){
      if(openglLib)dlclose(this->openglLib);
      this->openglLib = nullptr;
    }
};
#endif

void*getProcAddress(char const*name){
  static OpenGLFunctionLoader loader{};
  return loader(name);
}

}
