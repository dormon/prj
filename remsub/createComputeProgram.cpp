#include <createComputeProgram.hpp>
#include <FunctionPrologue.h>
#include <iostream>
#include <geGL/Program.h>
#include <geGL/StaticCalls.h>
#include <glm/glm.hpp>

void createComputeProgram(vars::Vars&vars){
  FUNCTION_PROLOGUE("remsub","computeSource");

  std::cerr << "createProgram" << std::endl;

  auto mpSrc = vars.getString("computeSource");
  auto prg = vars.reCreate<ge::gl::Program>(
      "program",
      std::make_shared<ge::gl::Shader>(GL_COMPUTE_SHADER,mpSrc)
      );
  prg->setNonexistingUniformWarning(false);
  auto info = prg->getInfo();
  vars.erase("uniforms");
  for(auto const&u:info->uniforms){
    auto const&name = u.first;
    if(name.find("[")!=std::string::npos)continue;
    auto const&unif = u.second;
    auto type = std::get<ge::gl::ProgramInfo::TYPE>(unif);
    switch(type){
      case GL_UNSIGNED_INT:
        {
          uint32_t v;
          ge::gl::glGetUniformuiv(prg->getId(),prg->getUniformLocation(name),&v);
          vars.addUint32("uniforms."+name,v);break;
        }
      case GL_INT:
        {
          int v;
          ge::gl::glGetUniformiv(prg->getId(),prg->getUniformLocation(name),(int*)&v);
          vars.addInt32("uniforms."+name,v);break;
        }
      case GL_INT_VEC2:
        {
          glm::ivec2 v;
          ge::gl::glGetUniformiv(prg->getId(),prg->getUniformLocation(name),(int*)&v);
          vars.add<glm::ivec2>("uniforms."+name,v);break;
        }
      case GL_FLOAT       :
        {
          float v;
          ge::gl::glGetUniformfv(prg->getId(),prg->getUniformLocation(name),&v);
          vars.addFloat ("uniforms."+name,v);break;
        }
      case GL_BOOL:
        {
          bool v;
          ge::gl::glGetUniformiv(prg->getId(),prg->getUniformLocation(name),(int*)&v);
          vars.addBool ("uniforms."+name,v);break;
        }
      case GL_FLOAT_VEC2:
        {
          glm::vec2 v;
          ge::gl::glGetUniformfv(prg->getId(),prg->getUniformLocation(name),(float*)&v);
          vars.add<glm::vec2>("uniforms."+name,v);break;
        }
      default:break;


    }


  }
}


