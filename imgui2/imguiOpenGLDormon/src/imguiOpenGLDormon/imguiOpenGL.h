#pragma once

#include<memory>
#include<imguiOpenGLDormon/imguiopengldormon_export.h>

class ImguiOpenGLImpl;
struct ImDrawData;
class IMGUIOPENGLDORMON_EXPORT ImguiOpenGL{
  public:
    ImguiOpenGL(char const*glsl_version = "#version 150");
    ~ImguiOpenGL();
    void render(ImDrawData*data);
  private:
    friend class ImguiOpenGLImpl;
    std::unique_ptr<ImguiOpenGLImpl>impl;
};

