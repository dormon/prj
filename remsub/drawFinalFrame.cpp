#include <drawFinalFrame.hpp>
#include <FunctionPrologue.h>

#include <createDrawProgram.hpp>
#include <geGL/StaticCalls.h>
#include <geGL/geGL.h>
#include <Project.hpp>

using DVars = VarsGLMDecorator<vars::Vars>;

void drawFinalFrame(DVars&vars){
  FUNCTION_CALLER();
  createDrawProgram(vars);
  ge::gl::glClearColor(0.1f,0.1f,0.1f,1.f);
  ge::gl::glClear(GL_COLOR_BUFFER_BIT);

  auto vao = vars.get<ge::gl::VertexArray>("emptyVao");
  auto prg = vars.get<ge::gl::Program>("drawProgram");

  vao->bind();
  auto proj = vars.get<Project>("project");
  if(proj->videoManager.finalFrame)
    proj->videoManager.finalFrame->bind(0);

  prg->use();
  prg->set2uiv("windowSize",(uint32_t*)&(vars.getUVec2("windowSize")));
  prg->set1f("zoom",vars.getFloat("view.zoom"));
  prg->set2fv("offset",(float*)vars.get<glm::vec2>("view.offset"));

  ge::gl::glDrawArrays(GL_TRIANGLE_STRIP,0,4);

  vao->unbind();
}
