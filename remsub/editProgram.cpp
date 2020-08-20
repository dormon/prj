#include <editProgram.hpp>

#include <cstring>

#include <imguiDormon/imgui.h>

#include <FunctionPrologue.h>

#include <Project.hpp>
#include <saveFragment.hpp>
#include <loadFragment.hpp>

void editProgram(vars::Vars&vars){
  FUNCTION_CALLER();

  auto&src = vars.getString("computeSource");
  static char text[1024*100];
  std::strcpy(text,src.data());

  static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
  ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 40), flags);
  src = text;

  ImGui::Columns(3,"col",false);
  if(ImGui::Button("comp")){
    vars.updateTicks("computeSource");
  }
  ImGui::NextColumn();
  if(ImGui::Button("save")){
    saveFragment(vars);
  }
  ImGui::NextColumn();
  if(ImGui::Button("load")){
    loadFragment(vars);
  }
  ImGui::Columns(1);

  auto proj = vars.get<Project>("project");
  proj->videoManager.setFrame();
  proj->videoManager.showGUI();

}
