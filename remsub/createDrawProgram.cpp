#include <createDrawProgram.hpp>
#include <FunctionPrologue.h>
#include <geGL/Program.h>

void createDrawProgram(vars::Vars&vars){
  FUNCTION_PROLOGUE("remsub");
  std::string const vsSrc = R".(
  #version 450

  layout(binding=0)uniform usampler2D finalFrame;

  uniform uvec2 windowSize = uvec2(512);
  uniform float zoom       = 1.f;
  uniform vec2 offset      = vec2(0);

  out vec2 vCoord;

  void main(){
    ivec2 frameSize = textureSize(finalFrame,0).xy;

    //vec2 scale = zoom*vec2(frameSize) / vec2(windowSize);
    vec2 scale = zoom*vec2(720,480) / vec2(windowSize);

    vCoord = vec2(gl_VertexID&1,gl_VertexID>>1);
    gl_Position = vec4((-1+2*vCoord)*scale+offset,0,1);
    vCoord.y = 1-vCoord.y;
  }

  ).";

  auto fsSrc = R".(
  #version 450

  layout(binding=0)uniform usampler2D finalFrame;

  in vec2 vCoord;
  out vec3 fColor;

  void main(){
    fColor = vec3(texture(finalFrame,vCoord).xyz/vec3(255));
    //fColor.x=1.f;
  }

  ).";
  auto prg = vars.reCreate<ge::gl::Program>(
      "drawProgram",
      std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER  ,vsSrc),
      std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER,fsSrc)
      );
}
