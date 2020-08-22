#include <init.hpp>

#include <SDL_keycode.h>

#include <Vars/Vars.h>

#include <Project.hpp>
#include <loadFragment.hpp>

void init(DVars&vars){
  auto proj = vars.add<Project>("project");
  if(vars.getString("args.projectName") != "")
    proj->load(vars.getString("args.projectName"));

  vars.add<ge::gl::VertexArray>("emptyVao");
  vars.addMap<SDL_Keycode, bool>("input.keyDown");

  vars.addBool("compile");

  vars.addString("computeSourceFile","../drawVideo.fp");
  loadFragment(vars);

  vars.addFloat("view.zoom",1.f);
  vars.add<glm::vec2>("view.offset",glm::vec2(0.f));
}
