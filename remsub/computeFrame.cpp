#include <computeFrame.hpp>
#include <FunctionPrologue.h>
#include <createComputeProgram.hpp>
#include <geGL/geGL.h>

#include <Project.hpp>

void createAuxBuffer(DVars&vars){
  FUNCTION_PROLOGUE("remsub");
  vars.reCreate<ge::gl::Buffer>("auxBuffer",sizeof(uint32_t)*1000);
}

void computeFrame(DVars&vars){
  FUNCTION_CALLER();

  createComputeProgram(vars);
  createAuxBuffer(vars);
  auto prg = vars.get<ge::gl::Program>("program");
  std::vector<std::string>uniformNames;
  vars.getDir(uniformNames,"uniforms");
  for(auto const&n:uniformNames){
    auto vName = "uniforms."+n;
    auto const&type = vars.getType(vName);
    if(type == typeid(bool    ))prg->set1i (n,vars.getBool  (vName));
    if(type == typeid(uint32_t))prg->set1ui(n,vars.getUint32(vName));
    if(type == typeid( int32_t))prg->set1i (n,vars.getInt32 (vName));
    if(type == typeid(float   ))prg->set1f (n,vars.getFloat (vName));
    if(type == typeid(glm::vec2))prg->set2fv(n,(float*)&vars.getVec2(vName));
  }
  auto proj = vars.get<Project>("project");
  proj->videoManager.bind();
  if(proj->videoManager.finalFrame)
    proj->videoManager.finalFrame->bindImage(0);
  prg->set1ui("activeClip",proj->videoManager.activeClip);
  proj->videoManager.setContrast(&*prg);
  proj->videoManager.setOffsetScale(&*prg);
  proj->videoManager.setSubBox(&*prg);

  auto auxBuffer = vars.get<ge::gl::Buffer>("auxBuffer");
  auxBuffer->clear(GL_R32UI,GL_RED_INTEGER,GL_UNSIGNED_INT);
  prg->bindBuffer("AuxBuffer",auxBuffer);
  prg->use();

  ge::gl::glDispatchCompute(vars.addOrGetInt32("wg",256),1,1);
}
