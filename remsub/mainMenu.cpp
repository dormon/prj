#include <mainMenu.hpp>
#include <Vars/Vars.h>
#include <imguiDormon/imgui.h>
#include <Project.hpp>

void mainMenu(vars::Vars&vars){
  if(ImGui::BeginMainMenuBar()){
    if(ImGui::BeginMenu("file")){
      if(ImGui::MenuItem("open")){
      }
      if(ImGui::MenuItem("save")){
        vars.get<Project>("project")->save(vars.getString("projectName"));
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}
