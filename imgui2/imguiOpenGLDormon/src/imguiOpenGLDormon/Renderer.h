#pragma once

#include<memory>
#include<imguiOpenGLDormon/imguiopengldormon_export.h>

struct ImDrawData;

namespace imguiOpenGLDormon{
  class RendererImpl;
  class Renderer;
}

struct ImDrawData;
class IMGUIOPENGLDORMON_EXPORT imguiOpenGLDormon::Renderer{
  public:
    Renderer(size_t version = 150);
    ~Renderer();
    void render(ImDrawData*data);
  private:
    friend class ImguiOpenGLImpl;
    std::unique_ptr<RendererImpl>impl;
};

