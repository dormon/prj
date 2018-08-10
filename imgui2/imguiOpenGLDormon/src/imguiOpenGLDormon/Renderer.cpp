#include <imguiOpenGLDormon/Renderer.h>
#include <imguiOpenGLDormon/RendererImpl.h>

namespace imguiOpenGLDormon{

Renderer::Renderer(size_t glsl_version){
  impl = std::make_unique<RendererImpl>(glsl_version);
}

Renderer::~Renderer(){}

void Renderer::render(ImDrawData*data){
  impl->render(data);
}

}
